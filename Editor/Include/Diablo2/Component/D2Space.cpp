#include "D2Space.h"
#include "D2Room.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/SpaceConstantBuffer.h"
#include "Component/CameraComponent.h"
#include "Engine.h"

CD2Space::CD2Space()	:
	mOwner(nullptr),
	mParent(nullptr),
	mChild1(nullptr),
	mChild2(nullptr),
	meSplitDir(eSplitDir::None),
	mRoom(nullptr),
	mSizeX(0),
	mSizeY(0),
	mSpaceID(-1),
	mLBIdxX(-1),
	mLBIdxY(-1),
	mLBIdx(-1),
	mRTIdxX(-1),
	mRTIdxY(-1),
	mRTIdx(-1),
	mCenterIdxX(-1),
	mCenterIdxY(-1),
	mCenterIdx(-1),
	mLevel(0),
	mbConnect(false),
	mCBuffer(nullptr)
{
}

CD2Space::~CD2Space()
{
	SAFE_DELETE(mRoom);
	SAFE_DELETE(mCBuffer);
}

bool CD2Space::Init()
{
	if (eTileShape::Rect == mOwner->GetTileShape())
	{
		mMesh = mOwner->GetScene()->GetResource()->FindMesh("RectSpaceMesh");
		mShader = CResourceManager::GetInst()->FindShader("SpaceRectShader");
	}
	else
	{
		mMesh = mOwner->GetScene()->GetResource()->FindMesh("ParticlePointMesh");
		mShader = CResourceManager::GetInst()->FindShader("SpaceRhombusShader");
	}

	mCBuffer = new CSpaceConstantBuffer;
	mCBuffer->Init();
	mCBuffer->SetColor(0.f, 0.f, 1.f, 1.f);

	return true;
}

void CD2Space::Render()
{
	CCameraComponent* cam = mOwner->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix matView, matProj;
	matView = cam->GetViewMatrix();
	matProj = cam->GetProjMatrix();

	Matrix matScale, matTrans;

	if (eTileShape::Rect == mOwner->GetTileShape())
	{
		matScale.Scaling(mOwner->GetTileSize() * Vector3(mSizeX, mSizeY, 0.f));
		matTrans.Translation(mOwner->GetTileSize() * Vector3(mLBIdxX, mLBIdxY, 0.f));

		Matrix matWorld, matWVP;
		matWorld = matScale * matTrans;
		matWVP = matWorld * matView * matProj;
		matWVP.Transpose();
		mCBuffer->SetMatWVP(matWVP);
	}
	else
	{
		Matrix matVP;
		matVP = matView * matProj;
		matVP.Transpose();
		mCBuffer->SetMatVP(matVP);

		float tileDiagonal = mOwner->GetTileDiagonal();

		CD2Space* neighbor = GetNeighbor();

		int lbX = mLBIdxX;
		int lbY = mLBIdxY;
		int rtX = mRTIdxX;
		int rtY = mRTIdxY;
		int offset = tileDiagonal * 0.5f;
		
		// LB
		Vector3 pos = Vector3(lbX * tileDiagonal + offset, lbY * tileDiagonal+ offset, 0.f).TransformCoord(mOwner->GetMatIsoToWorld());
		mCBuffer->SetRhombusLB(pos);
		// LT
		pos = Vector3(lbX * tileDiagonal+ offset, rtY * tileDiagonal+ offset, 0.f).TransformCoord(mOwner->GetMatIsoToWorld());
		mCBuffer->SetRhombusLT(pos);
		// RT
		pos = Vector3(rtX * tileDiagonal+ offset, rtY * tileDiagonal+ offset, 0.f).TransformCoord(mOwner->GetMatIsoToWorld());
		mCBuffer->SetRhombusRT(pos);
		// RB
		pos = Vector3(rtX * tileDiagonal+ offset, lbY * tileDiagonal+ offset, 0.f).TransformCoord(mOwner->GetMatIsoToWorld());
		mCBuffer->SetRhombusRB(pos);
	}

	mCBuffer->UpdateCBuffer();
	mShader->SetShader();
	mMesh->Render();

	if (mChild1)
	{
		mChild1->Render();
	}
	if (mChild2)
	{
		mChild2->Render();
	}
}

void CD2Space::StartPartitioning(const int sizeX, const int sizeY, const int maxLevel)
{
	if (!mParent)
	{
		mLBIdxX = 0;
		mLBIdxY = 0;
		mLBIdx = 0;

		mRTIdxX = sizeX - 1;
		mRTIdxY = sizeY - 1;
		mRTIdx = sizeX * sizeY - 1;

		mCenterIdxX = sizeX / 2;
		mCenterIdxY = sizeY / 2;
		mCenterIdx = mCenterIdxY * mOwner->GetMapCountX() + mCenterIdxX;

		mSizeX = sizeX;
		mSizeY = sizeY;
	}
	
	// 분할시작
	doPartitioning(maxLevel);
}

bool CD2Space::MakeRoom()
{
	if (mChild1)
	{
		mChild1->MakeRoom();
	}
	if (mChild2)
	{
		mChild2->MakeRoom();
	}
	// 이 공간이 더 이상 분할되지 않은 공간이면 방 생성한다.
	else
	{
		mRoom = new CD2Room;
		mRoom->mSpace = this;

		// 길 생성을 위해 공간 경계에서 2칸은 띄워줘야 함
		int minIdxX = mLBIdxX + 2;
		int minIdxY = mLBIdxY + 2;
		int maxIdxX = mRTIdxX - 2;
		int maxIdxY = mRTIdxY - 2;

		if (maxIdxX - minIdxX + 1 <= mOwner->GetRoomSizeMin())
		{
			mRoom->mLBIdxX = minIdxX;
			mRoom->mSizeX = mOwner->GetRoomSizeMin();
			mRoom->mRTIdxX = minIdxX + mOwner->GetRoomSizeMin() - 1;
		}
		else
		{
			int maxLBX = maxIdxX - mOwner->GetRoomSizeMin();
			int range = (maxLBX - minIdxX + 1) % 5 + 1;
			mRoom->mLBIdxX = minIdxX + floor(rand() % range);

			int minRTX = mRoom->mLBIdxX + mOwner->GetRoomSizeMin() - 1;
			range = (maxIdxX - minRTX) % 5 + 1;
			mRoom->mRTIdxX = maxIdxX - floor(rand() % range);

			mRoom->mSizeX = mRoom->mRTIdxX - mRoom->mLBIdxX + 1;
		}

		if (maxIdxY - minIdxY + 1 <= mOwner->GetRoomSizeMin())
		{
			mRoom->mLBIdxY = minIdxY;
			mRoom->mSizeY = mOwner->GetRoomSizeMin();
			mRoom->mRTIdxY = minIdxY + mOwner->GetRoomSizeMin() - 1;
		}
		else
		{
			int maxLBY = maxIdxY - mOwner->GetRoomSizeMin();
			int range = (maxLBY - minIdxY + 1) % 5 + 1;
			mRoom->mLBIdxY = minIdxY + floor(rand() % range);

			int minRTY = mRoom->mLBIdxY + mOwner->GetRoomSizeMin() - 1;
			range = (maxIdxY - minRTY) % 5 + 1;
			mRoom->mRTIdxY = maxIdxY - floor(rand() % range);

			mRoom->mSizeY = mRoom->mRTIdxY - mRoom->mLBIdxY + 1;
		}

		mRoom->mRTIdx = mRoom->mRTIdxY * mOwner->GetMapCountX() + mRoom->mRTIdxX;
		mRoom->mLBIdx = mRoom->mLBIdxY * mOwner->GetMapCountX() + mRoom->mLBIdxX;

		if (mRoom->mRTIdxX >= mOwner->GetMapCountX() || mRoom->mRTIdxY >= mOwner->GetMapCountY() ||
			mRoom->mLBIdxX >= mOwner->GetMapCountX() || mRoom->mLBIdxY >= mOwner->GetMapCountY())
		{
			assert(false);
		}

		// 중앙 인덱스
		mRoom->mCenterIdxX = mRoom->mLBIdxX + (mRoom->mSizeX / 2);
		mRoom->mCenterIdxY = mRoom->mLBIdxY + (mRoom->mSizeY / 2);
		mRoom->mCenterIdx = mRoom->mCenterIdxY * mOwner->GetMapCountX() + mRoom->mCenterIdxX;

		mOwner->AddRoom(mRoom);
	}
	return true;
}

void CD2Space::ConnectSpace()
{
	// 최상위 공간인 경우
	if (!mParent)
	{
		return;
	}

	CD2Space* neighbor = GetNeighbor();
	mbConnect = true;
	neighbor->mbConnect = true;

	// 두 공간이 세로로 나뉘어진 공간에 속할 경우
	if (eSplitDir::Vertical == meSplitDir)
	{
		// 이 공간이 왼쪽 공간인 경우
		if (mCenterIdxX < neighbor->mCenterIdxX)
		{
			// 이 공간의 가장 오른쪽에 있는 방과, 그 방에서 가장 가까운 이웃 공간의 방을 찾는다. 
			CD2Room* mostRight = GetRightMostRoomInfo();
			CD2Room* closeRoom = neighbor->GetMostCloseRoomInfo(mostRight->mCenterIdx);

			// 직선으로 이어도 되는 경우
			if (mostRight->mRTIdxY - 2 >= closeRoom->mLBIdxY + 1 && mostRight->mLBIdxY + 2 <= closeRoom->mRTIdxY - 1)
			{
				// 이 방의 오른쪽 위에서 통로 생성
				if (mostRight->mRTIdxY - 2 <= closeRoom->mRTIdxY - 2)
				{
					// 통로 생성
					for (int x = mostRight->mRTIdxX + 1; x <= closeRoom->mLBIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mostRight->mRTIdxY - 1, eTileType::WallNW);
						mOwner->SetTileType(x, mostRight->mRTIdxY - 2, eTileType::WallSE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mRTIdxY - 1, eTileType::EntryNELeft);
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mRTIdxY - 2, eTileType::EntryNERight);
					mOwner->SetTileType(closeRoom->mLBIdxX, mostRight->mRTIdxY - 1, eTileType::EntrySWLeft);
					mOwner->SetTileType(closeRoom->mLBIdxX, mostRight->mRTIdxY - 2, eTileType::EntrySWRight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mRTIdxY - 2, eTileType::WallEndSWBottom);
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mRTIdxY - 1, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, mostRight->mRTIdxY - 2, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, mostRight->mRTIdxY - 1, eTileType::WallEndNETop);
				}
				// 이 방의 오른쪽 아래에서 통로 생성
				else
				{	
					// 통로 생성
					for (int x = mostRight->mRTIdxX + 1; x <= closeRoom->mLBIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mostRight->mLBIdxY + 2, eTileType::WallNW);
						mOwner->SetTileType(x, mostRight->mLBIdxY + 1, eTileType::WallSE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mLBIdxY + 2, eTileType::EntryNELeft);
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mLBIdxY + 1, eTileType::EntryNERight);
					mOwner->SetTileType(closeRoom->mLBIdxX, mostRight->mLBIdxY + 2, eTileType::EntrySWLeft);
					mOwner->SetTileType(closeRoom->mLBIdxX, mostRight->mLBIdxY + 1, eTileType::EntrySWRight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mLBIdxY + 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mLBIdxY + 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, mostRight->mLBIdxY + 1, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, mostRight->mLBIdxY + 2, eTileType::WallEndNETop);
				}
			}
			// 통로를 꺾어서 이어야 하는 경우
			else
			{
				// 1. 이 공간의 방이 이어야 할 방보다 낮게 있는 경우
				if (mostRight->mRTIdxY - closeRoom->mLBIdxY < 3)
				{
					// 통로 생성
					for (int y = mostRight->mRTIdxY - 1; y <= closeRoom->mLBIdxY + 1; ++y)
					{
						mOwner->SetTileType(mRTIdxX, y, eTileType::WallSW);
						mOwner->SetTileType(neighbor->mLBIdxX, y, eTileType::WallNE);
					}

					// 코너 배치
					mOwner->SetTileType(mRTIdxX, closeRoom->mLBIdxY - 2, eTileType::CornerW);
					mOwner->SetTileType(neighbor->mLBIdxX, mostRight->mRTIdxY - 2, eTileType::CornerE);

					// 진입점 생성
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mRTIdxY - 2, eTileType::EntryNERight);
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mRTIdxY - 1, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->mLBIdxX, closeRoom->mLBIdxY + 2, eTileType::EntrySWLeft);
					mOwner->SetTileType(closeRoom->mLBIdxX, closeRoom->mLBIdxY + 1, eTileType::EntrySWRight);

					// 방에서 바로 이어지는 통로
					for (int x = mostRight->mRTIdxX + 1; x <= mRTIdxX; ++x)
					{
						mOwner->SetTileType(x, mostRight->mRTIdxY - 2, eTileType::WallSE);
					}
					for (int x = mostRight->mRTIdxX + 1; x < mRTIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mostRight->mRTIdxY - 1, eTileType::WallNW);
					}
					for (int x = closeRoom->mLBIdxX - 1; x >= neighbor->mLBIdxX; --x)
					{
						mOwner->SetTileType(x, closeRoom->mLBIdxY + 2, eTileType::WallNW);
					}
					for (int x = closeRoom->mLBIdxX - 1; x > neighbor->mLBIdxX + 1; --x)
					{
						mOwner->SetTileType(x, closeRoom->mLBIdxY + 1, eTileType::WallSE);
					}

					// 통로 교차점
					mOwner->SetTileType(mRTIdxX - 1, mostRight->mRTIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(neighbor->mLBIdxX + 1, closeRoom->mLBIdxY + 1, eTileType::WallEndSEBottom);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mRTIdxY - 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mRTIdxY - 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, closeRoom->mLBIdxY + 2, eTileType::WallEndNETop);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, closeRoom->mLBIdxY + 1, eTileType::WallEndNEBottom);

					// 안쪽 코너
					mOwner->SetTileType(mRTIdxX, mostRight->mRTIdxX - 1, eTileType::InnerCornerW);
					mOwner->SetTileType(neighbor->mLBIdxX, closeRoom->mLBIdxY + 1, eTileType::InnerCornerE);
				}
				// 2. 이 공간의 방이 이어야 할 방보다 높게 있는 경우
				else if (closeRoom->mRTIdxY - mostRight->mLBIdxY < 3)
				{
					// 1. 통로 생성
					for (int y = mostRight->mLBIdxY + 1; y >= closeRoom->mRTIdxY - 1; --y)
					{
						mOwner->SetTileType(mRTIdxX, y, eTileType::WallSW);
						mOwner->SetTileType(neighbor->mLBIdxX, y, eTileType::WallNE);
					}

					// 2 통로 코너 배치
					mOwner->SetTileType(neighbor->mLBIdxX, mostRight->mLBIdxY + 2, eTileType::CornerN);
					mOwner->SetTileType(mRTIdxX, closeRoom->mRTIdxY - 2, eTileType::CornerS);

					// 3. 진입점 생성
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mLBIdxY - 2, eTileType::EntryNELeft);
					mOwner->SetTileType(mostRight->mRTIdxX, mostRight->mLBIdxY - 1, eTileType::EntryNERight);
					mOwner->SetTileType(closeRoom->mLBIdxX, closeRoom->mRTIdxY - 2, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->mLBIdxX, closeRoom->mRTIdxY - 1, eTileType::EntrySWLeft);

					// 4. 방에서 바로 이어지는 통로
					for (int x = mostRight->mRTIdxX + 1; x <= mRTIdxX; ++x)
					{
						mOwner->SetTileType(x, mostRight->mLBIdxY - 2, eTileType::WallNW);
					}
					for (int x = mostRight->mRTIdxX + 1; x < mRTIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mostRight->mLBIdxY - 1, eTileType::WallSE);
					}
					for (int x = closeRoom->mLBIdxX - 1; x >= neighbor->mLBIdxX; --x)
					{
						mOwner->SetTileType(x, closeRoom->mRTIdxY - 2, eTileType::WallSE);
					}
					for (int x = closeRoom->mLBIdxX - 1; x > neighbor->mLBIdxX; --x)
					{
						mOwner->SetTileType(x, closeRoom->mRTIdxY - 1, eTileType::WallNW);
					}
					
					// 5. 통로 교차점
					mOwner->SetTileType(mRTIdxX - 1, mostRight->mLBIdxY + 1, eTileType::WallEndNEBottom);
					mOwner->SetTileType(neighbor->mLBIdxX - 1, closeRoom->mRTIdxY - 1, eTileType::WallEndSWTop);

					// 6. 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mLBIdxY + 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(mostRight->mRTIdxX + 1, mostRight->mLBIdxY + 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, closeRoom->mRTIdxY - 2, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, closeRoom->mRTIdxY - 1, eTileType::WallEndNETop);

					// 7. 안쪽 코너
					mOwner->SetTileType(mRTIdxX, mostRight->mLBIdxY + 1, eTileType::InnerCornerS);
					mOwner->SetTileType(neighbor->mLBIdxX, closeRoom->mRTIdxY - 1, eTileType::InnerCornerN);
				}
			}
		}
		// 오른쪽인 경우
		else
		{
			CD2Room* mostLeft = GetLeftMostRoomInfo();
			CD2Room* closeRoom = neighbor->GetMostCloseRoomInfo(mostLeft->mCenterIdx);

			// 직선으로 이어도 되는 경우
			if (mostLeft->mLBIdxY - 2 <= closeRoom->mRTIdxY - 1 && mostLeft->mRTIdxY - 2 >= closeRoom->mLBIdxY + 1)
			{
				// 이 방의 왼쪽 위에서 통로 생성
				if (mostLeft->mRTIdxY - 2 <= closeRoom->mRTIdxY - 2)
				{
					// 통로 생성
					for (int x = mostLeft->mLBIdxX - 1; x >= closeRoom->mRTIdxX + 1; --x)
					{
						mOwner->SetTileType(x, mostLeft->mRTIdxY - 1, eTileType::WallNW);
						mOwner->SetTileType(x, mostLeft->mRTIdxY - 2, eTileType::WallSE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mRTIdxY - 1, eTileType::EntrySWLeft);
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mRTIdxY - 2, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->mRTIdxX, mostLeft->mRTIdxY - 1, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->mRTIdxX, mostLeft->mRTIdxY - 2, eTileType::EntryNERight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mRTIdxY - 1, eTileType::WallEndNETop);
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mRTIdxY - 2, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, mostLeft->mRTIdxY - 1, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, mostLeft->mRTIdxY - 2, eTileType::WallEndSWBottom);
				}
				else
				{
					// 통로 생성
					for (int x = mostLeft->mLBIdxX - 1; x >= closeRoom->mRTIdxX + 1; --x)
					{
						mOwner->SetTileType(x, mostLeft->mLBIdxY + 2, eTileType::WallNW);
						mOwner->SetTileType(x, mostLeft->mLBIdxY + 1, eTileType::WallSE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mLBIdxY + 2, eTileType::EntrySWLeft);
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mLBIdxY + 1, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->mRTIdxX, mostLeft->mLBIdxY + 2, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->mRTIdxX, mostLeft->mLBIdxY + 1, eTileType::EntryNERight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mLBIdxY + 2, eTileType::WallEndNETop);
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mLBIdxY + 1, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, mostLeft->mLBIdxY + 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, mostLeft->mLBIdxY + 1, eTileType::WallEndSWBottom);
				}
			}
			// 통로를 꺾어서 이어야 하는 경우
			else
			{
				// 1. 이 공간의 방이 이어야 할 방보다 낮게 있는 경우
				if (mostLeft->mRTIdxY - closeRoom->mLBIdxY < 3)
				{
					// 통로 생성
					for (int y = mostLeft->mRTIdxY - 1; y <= closeRoom->mLBIdxY + 1; ++y)
					{
						mOwner->SetTileType(mLBIdxX, y, eTileType::WallNE);
						mOwner->SetTileType(neighbor->mRTIdxX, y, eTileType::WallSW);
					}

					// 통로 코너 배치
					mOwner->SetTileType(neighbor->mRTIdxX, mostLeft->mRTIdxY - 2, eTileType::CornerS);
					mOwner->SetTileType(mLBIdxX, closeRoom->mLBIdxY + 2, eTileType::CornerN);

					// 진입점 생성
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mRTIdxY - 1, eTileType::EntrySWLeft);
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mRTIdxY - 2, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->mRTIdxX, closeRoom->mLBIdxY + 1, eTileType::EntryNERight);
					mOwner->SetTileType(closeRoom->mRTIdxX, closeRoom->mLBIdxY + 2, eTileType::EntryNELeft);

					// 방에서 바로 이어지는 통로
					for (int x = mostLeft->mLBIdxX - 1; x >= mLBIdxX; --x)
					{
						mOwner->SetTileType(x, mostLeft->mRTIdxY - 2, eTileType::WallSE);
					}
					for (int x = mostLeft->mLBIdxX - 1; x > mLBIdxX + 1; --x)
					{
						mOwner->SetTileType(x, mostLeft->mRTIdxY - 1, eTileType::WallNW);
					}
					for (int x = closeRoom->mRTIdxX + 1; x <= neighbor->mRTIdxX; ++x)
					{
						mOwner->SetTileType(x, closeRoom->mLBIdxY + 2, eTileType::WallNW);
					}
					for (int x = closeRoom->mRTIdxX + 1; x < neighbor->mRTIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, closeRoom->mLBIdxY + 1, eTileType::WallSE);
					}
					
					// 통로 교차점
					mOwner->SetTileType(mLBIdxX + 1, mostLeft->mRTIdxY - 1, eTileType::WallEndSWTop);
					mOwner->SetTileType(neighbor->mRTIdxX - 1, closeRoom->mLBIdxX + 1, eTileType::WallEndNEBottom);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mRTIdxY - 1, eTileType::WallEndNETop);
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mRTIdxY - 2, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, closeRoom->mLBIdxY + 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, closeRoom->mLBIdxY + 2, eTileType::WallEndSWTop);
					
					// 안쪽 코너
					mOwner->SetTileType(mLBIdxX, mostLeft->mRTIdxY - 1, eTileType::InnerCornerN);
					mOwner->SetTileType(neighbor->mRTIdxX, closeRoom->mLBIdxX + 1, eTileType::InnerCornerS);
				}
				// 2. 이 공간의 방이 이어야 할 방보다 높게 있는 경우
				else if (closeRoom->mRTIdxY - mostLeft->mLBIdxY < 3)
				{
					// 통로 생성
					for (int y = mostLeft->mLBIdxY + 1; y >= closeRoom->mRTIdxY - 1; --y)
					{
						mOwner->SetTileType(mLBIdxX, y, eTileType::WallNE);
						mOwner->SetTileType(neighbor->mRTIdxX, eTileType::WallSW);
					}

					// 통로 코너 배치
					mOwner->SetTileType(mLBIdxX, closeRoom->mRTIdxY - 2, eTileType::CornerE);
					mOwner->SetTileType(neighbor->mRTIdxX, mostLeft->mLBIdxY + 2, eTileType::CornerW);

					// 진입점 생성
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mLBIdxX + 1, eTileType::EntrySWRight);
					mOwner->SetTileType(mostLeft->mLBIdxX, mostLeft->mLBIdxX + 2, eTileType::EntrySWLeft);
					mOwner->SetTileType(closeRoom->mRTIdxX, closeRoom->mRTIdxY - 1, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->mRTIdxX, closeRoom->mRTIdxY - 2, eTileType::EntryNERight);

					// 방에서 바로 이어지는 통로
					for (int x = mostLeft->mLBIdxX - 1; x >= mLBIdxX; --x)
					{
						mOwner->SetTileType(x, mostLeft->mLBIdxY + 2, eTileType::WallNW);
					}
					for (int x = mostLeft->mLBIdxX - 1; x > mLBIdxX - 1; --x)
					{
						mOwner->SetTileType(x, mostLeft->mLBIdxY + 1, eTileType::WallSE);
					}
					for (int x = closeRoom->mRTIdxX + 1; x <= neighbor->mRTIdxX; ++x)
					{
						mOwner->SetTileType(x, closeRoom->mRTIdxY - 2, eTileType::WallSE);
					}
					for (int x = closeRoom->mRTIdxX + 1; x < neighbor->mRTIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, closeRoom->mRTIdxY - 1, eTileType::WallNW);
					}
					
					// 통로 교차점
					mOwner->SetTileType(mLBIdxX + 1, mostLeft->mLBIdxY + 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(neighbor->mRTIdxX - 1, closeRoom->mRTIdxY - 1, eTileType::WallEndNETop);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mLBIdxY + 2, eTileType::WallEndNETop);
					mOwner->SetTileType(mostLeft->mLBIdxX - 1, mostLeft->mLBIdxY + 1, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, closeRoom->mRTIdxY - 1, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->mRTIdxX + 1, closeRoom->mRTIdxY - 2, eTileType::WallEndSWBottom);

					// 안쪽 코너
					mOwner->SetTileType(mLBIdxX, mostLeft->mLBIdxY + 1, eTileType::InnerCornerE);
					mOwner->SetTileType(neighbor->mRTIdxX, closeRoom->mRTIdxY - 1, eTileType::InnerCornerW);
				}
			}
		}
	}
	// 두 공간이 가로로 나뉘어진 공간에 속할 경우
	else
	{
		// 이 공간이 아래쪽 공간인 경우
		if (mCenterIdxY < neighbor->mCenterIdxY)
		{
			CD2Room* mostTop = GetTopMostRoomInfo();
			CD2Room* closeRoom = neighbor->GetMostCloseRoomInfo(mostTop->mCenterIdx);

			// 직선으로 이어도 되는 경우
			if (mostTop->mLBIdxX + 2 <= closeRoom->mRTIdxX - 1 && mostTop->mRTIdxX - 2 >= closeRoom->mLBIdxX + 1)
			{
				// 이 방의 오른쪽 위에서 통로 생성
				if (mostTop->mRTIdxX - 2 <= closeRoom->mRTIdxX - 2)
				{
					// 통로 생성
					for (int y = mostTop->mRTIdxY + 1; y <= closeRoom->mLBIdxY - 1; ++y)
					{
						mOwner->SetTileType(mostTop->mRTIdxX - 2, y, eTileType::WallSW);
						mOwner->SetTileType(mostTop->mRTIdxX - 1, y, eTileType::WallNE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostTop->mRTIdxX - 2, mostTop->mRTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostTop->mRTIdxX - 1, mostTop->mRTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(mostTop->mRTIdxX - 2, closeRoom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostTop->mRTIdxX - 1, closeRoom->mLBIdxY, eTileType::EntrySERight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostTop->mRTIdxX - 1, mostTop->mRTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostTop->mRTIdxX - 2, mostTop->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostTop->mRTIdxX - 1, closeRoom->mLBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostTop->mRTIdxX - 2, closeRoom->mLBIdxY - 1, eTileType::WallEndNWBottom);
				}
				// 이 방의 왼쪽 위에서 통로 생성
				else
				{
					for (int y = mostTop->mRTIdxY + 1; y <= closeRoom->mLBIdxY - 1; ++y)
					{
						mOwner->SetTileType(mostTop->mLBIdxX + 1, y, eTileType::WallSW);
						mOwner->SetTileType(mostTop->mLBIdxX + 2, y, eTileType::WallNE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostTop->mLBIdxX + 1, mostTop->mRTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostTop->mLBIdxX + 2, mostTop->mRTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(mostTop->mLBIdxX + 1, closeRoom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostTop->mLBIdxX + 2, closeRoom->mLBIdxY, eTileType::EntrySERight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostTop->mLBIdxX + 2, mostTop->mRTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostTop->mLBIdxX + 1, mostTop->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostTop->mLBIdxX + 2, closeRoom->mLBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostTop->mLBIdxX + 1, closeRoom->mLBIdxY - 1, eTileType::WallEndNWBottom);
				}
			}
			// 통로를 꺾어서 이어야 할 경우
			else
			{
				// 1. 이 공간의 방이 이웃 공간의 방보다 왼쪽에 있는 경우
				if (mostTop->mRTIdxX - closeRoom->mLBIdxX < 3)
				{
					// 통로 생성
					for (int x = mostTop->mRTIdxX - 1; x <= closeRoom->mLBIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mRTIdxY, eTileType::WallSE);
						mOwner->SetTileType(x, neighbor->mLBIdxY, eTileType::WallNW);
					}

					// 통로 코너 배치
					mOwner->SetTileType(mostTop->mRTIdxX - 2, neighbor->mLBIdxX, eTileType::CornerW);
					mOwner->SetTileType(closeRoom->mLBIdxX + 2, mRTIdxY, eTileType::CornerE);

					// 진입점 생성
					mOwner->SetTileType(mostTop->mRTIdxX - 2, mostTop->mRTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostTop->mRTIdxX - 1, mostTop->mRTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(closeRoom->mLBIdxX - 1, closeRoom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(closeRoom->mLBIdxX - 2, closeRoom->mLBIdxY, eTileType::EntrySERight);

					// 방에서 바로 이어지는 통로
					for (int y = mostTop->mRTIdxY + 1; y <= mRTIdxY; ++y)
					{
						mOwner->SetTileType(mostTop->mRTIdxX - 2, y, eTileType::WallSW);
					}
					for (int y = mostTop->mRTIdxY + 1; y < mRTIdxY - 1; ++y)
					{
						mOwner->SetTileType(mostTop->mRTIdxX - 1, y, eTileType::WallNE);
					}
					for (int y = closeRoom->mLBIdxY - 1; y >= neighbor->mLBIdxY; --y)
					{
						mOwner->SetTileType(closeRoom->mLBIdxX + 2, eTileType::WallNE);
					}
					for (int y = closeRoom->mLBIdxY - 1; y > neighbor->mLBIdxY + 1; --y)
					{
						mOwner->SetTileType(closeRoom->mLBIdxX + 1, eTileType::WallSW);
					}
					
					// 통로 교차점
					mOwner->SetTileType(mostTop->mRTIdxX - 1, mRTIdxY - 1, eTileType::WallEndNETop);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, neighbor->mLBIdxY - 1, eTileType::WallEndSEBottom);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostTop->mRTIdxX - 1, mostTop->mRTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostTop->mRTIdxX - 2, mostTop->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, closeRoom->mLBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX + 2, closeRoom->mLBIdxY - 1, eTileType::WallEndNWTop);

					// 내부 코너
					mOwner->SetTileType(mostTop->mRTIdxX - 1, mRTIdxY, eTileType::InnerCornerE);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, neighbor->mLBIdxY, eTileType::InnerCornerW);
				}

				// 2. 이 공간의 바이 이웃 공간의 방보다 오른쪽에 있는 경우
				if (closeRoom->mRTIdxX - mostTop->mLBIdxX < 3)
				{	
					// 통로 생성
					for (int x = closeRoom->mRTIdxX - 1; x <= mostTop->mLBIdxX + 1; ++x)
					{
						mOwner->SetTileType(x, neighbor->mLBIdxY, eTileType::WallNW);
						mOwner->SetTileType(x, mRTIdxY, eTileType::WallSE);
					}

					// 통로 코너 배치
					mOwner->SetTileType(mostTop->mLBIdxX + 2, neighbor->mLBIdxY, eTileType::CornerN);
					mOwner->SetTileType(closeRoom->mRTIdxX - 2, mLBIdxY, eTileType::CornerS);

					// 진입점 생성
					mOwner->SetTileType(mostTop->mLBIdxX + 1, mostTop->mRTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostTop->mLBIdxX + 2, mostTop->mRTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, closeRoom->mLBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(closeRoom->mRTIdxX - 2, closeRoom->mLBIdxY, eTileType::EntrySELeft);

					// 방에서 바로 이어지는 통로
					for (int y = mostTop->mRTIdxY + 1; y <= mRTIdxY; ++y)
					{
						mOwner->SetTileType(mostTop->mLBIdxX + 2, y, eTileType::WallNE);
					}
					for (int y = mostTop->mRTIdxY + 1; y < mRTIdxY - 1; ++y)
					{
						mOwner->SetTileType(mostTop->mLBIdxX + 1, y, eTileType::WallSW);
					}
					for (int y = closeRoom->mLBIdxY - 1; y >= neighbor->mLBIdxY; --y)
					{
						mOwner->SetTileType(closeRoom->mRTIdxX - 2, y, eTileType::WallSW);
					}
					for (int y = closeRoom->mLBIdxY - 1; y > neighbor->mLBIdxY + 1; --y)
					{
						mOwner->SetTileType(closeRoom->mRTIdxX - 1, y, eTileType::WallNE);
					}
					
					// 통로 교차점
					mOwner->SetTileType(mostTop->mLBIdxX + 1, mRTIdxY - 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, neighbor->mLBIdxY + 1, eTileType::WallEndNETop);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostTop->mLBIdxX + 2, mostTop->mRTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostTop->mLBIdxX + 1, mostTop->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX - 2, closeRoom->mLBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, closeRoom->mLBIdxY - 1, eTileType::WallEndNWTop);

					// 내부 코너
					mOwner->SetTileType(mostTop->mLBIdxX + 1, mRTIdxY, eTileType::InnerCornerS);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, neighbor->mLBIdxY, eTileType::InnerCornerN);
				}
			}
		}
		// 이 공간이 위쪽 공간인 경우
		else
		{
			CD2Room* mostBottom = GetBottomMostRoomInfo();
			CD2Room* closeRoom = neighbor->GetMostCloseRoomInfo(mostBottom->mCenterIdx);

			// 직선으로 이어도 되는 경우
			if (mostBottom->mLBIdxX + 1 <= closeRoom->mRTIdxX - 2 && mostBottom->mRTIdxX - 2 >= closeRoom->mLBIdxX + 1)
			{
				// 이 방의 오른쪽 아래에서 통로 생성
				if (mostBottom->mRTIdxX - 2 <= closeRoom->mRTIdxX - 2)
				{
					// 통로 생성
					for (int y = mostBottom->mLBIdxY - 1; y >= closeRoom->mRTIdxY + 1; --y)
					{
						mOwner->SetTileType(mostBottom->mRTIdxX - 2, y, eTileType::WallSW);
						mOwner->SetTileType(mostBottom->mRTIdxX - 1, y, eTileType::WallNE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, mostBottom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, mostBottom->mLBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, closeRoom->mRTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, closeRoom->mRTIdxY, eTileType::EntryNWRight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, mostBottom->mLBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, mostBottom->mLBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, closeRoom->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, closeRoom->mRTIdxY + 1, eTileType::WallEndSETop);
				}
				// 이 방의 왼쪽 아래에서 통로 생성
				else
				{
					// 통로 생성
					for (int y = mostBottom->mLBIdxY - 1; y >= closeRoom->mRTIdxY + 1; --y)
					{
						mOwner->SetTileType(mostBottom->mLBIdxX + 1, y, eTileType::WallSW);
						mOwner->SetTileType(mostBottom->mLBIdxX + 2, y, eTileType::WallNE);
					}
					
					// 진입점 배치
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, mostBottom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, mostBottom->mLBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, closeRoom->mRTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, closeRoom->mRTIdxY, eTileType::EntryNWRight);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, mostBottom->mLBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, mostBottom->mLBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, closeRoom->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, closeRoom->mRTIdxY + 1, eTileType::WallEndSETop);
				}
			}
			else
			{
				// 이 공간의 방이 이웃 공간의 방보다 왼쪽에 있는 경우
				if (mostBottom->mRTIdxX - closeRoom->mLBIdxX < 3)
				{
					// 통로 생성
					for (int x = mostBottom->mRTIdxX - 1; x <= closeRoom->mLBIdxX + 1; ++x)
					{
						mOwner->SetTileType(x, mLBIdxY, eTileType::WallNW);
						mOwner->SetTileType(x, neighbor->mRTIdxY, eTileType::WallSE);
					}

					// 통로 코너 배치
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, neighbor->mRTIdxY, eTileType::CornerS);
					mOwner->SetTileType(closeRoom->mLBIdxX + 2, mLBIdxY, eTileType::CornerN);

					// 진입점 생성
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, mostBottom->mLBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, mostBottom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, closeRoom->mRTIdxY, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->mLBIdxX + 2, closeRoom->mRTIdxY, eTileType::EntryNERight);

					// 방에서 바로 이어지는 통로
					for (int y = mostBottom->mLBIdxY - 1; y >= mLBIdxY; --y)
					{
						mOwner->SetTileType(mostBottom->mRTIdxX - 2, y, eTileType::WallSW);
					}
					for (int y = mostBottom->mLBIdxY - 1; y > mLBIdxY + 1; --y)
					{
						mOwner->SetTileType(mostBottom->mRTIdxX - 1, y, eTileType::WallNE);
					}
					for (int y = closeRoom->mRTIdxY + 1; y <= neighbor->mRTIdxY; ++y)
					{
						mOwner->SetTileType(closeRoom->mLBIdxX + 2, y, eTileType::WallNE);
					}
					for (int y = closeRoom->mRTIdxY + 1; y < neighbor->mRTIdxY - 1; ++y)
					{
						mOwner->SetTileType(closeRoom->mLBIdxX + 1, y, eTileType::WallSW);
					}
					
					// 통로 교차점
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, mLBIdxY + 1, eTileType::WallEndNETop);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, neighbor->mRTIdxY - 1, eTileType::WallEndNWBottom);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, mostBottom->mLBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostBottom->mRTIdxX - 2, mostBottom->mLBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, closeRoom->mRTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(closeRoom->mLBIdxX + 2, closeRoom->mRTIdxY + 1, eTileType::WallEndSETop);

					// 내부 코너
					mOwner->SetTileType(mostBottom->mRTIdxX - 1, mLBIdxY, eTileType::InnerCornerE);
					mOwner->SetTileType(closeRoom->mLBIdxX + 1, neighbor->mRTIdxY, eTileType::InnerCornerW);
				}

				// 2. 이 공간의 방이 이웃 공간의 방보다 오른쪽에 있는 경우
				if (closeRoom->mRTIdxX - mostBottom->mLBIdxX < 3)
				{	
					// 통로 생성
					for (int x = mostBottom->mLBIdxX + 1; x >= closeRoom->mRTIdxX - 1; --x)
					{
						mOwner->SetTileType(x, mLBIdxY, eTileType::WallNW);
						mOwner->SetTileType(x, neighbor->mRTIdxY, eTileType::WallSE);
					}

					// 통로 코너 배치
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, neighbor->mRTIdxY, eTileType::CornerE);
					mOwner->SetTileType(closeRoom->mRTIdxX - 2, mLBIdxY, eTileType::CornerW);

					// 진입점 생성
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, mostBottom->mLBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, mostBottom->mLBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, closeRoom->mRTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(closeRoom->mRTIdxX - 2, closeRoom->mRTIdxY, eTileType::EntryNWLeft);

					// 방에서 바로 이어지는 통로
					for (int y = mostBottom->mLBIdxY - 1; y >= mLBIdxY; --y)
					{
						mOwner->SetTileType(mostBottom->mLBIdxX + 2, y, eTileType::WallNE);
					}
					for (int y = mostBottom->mLBIdxY - 1; y > mLBIdxY - 1; --y)
					{
						mOwner->SetTileType(mostBottom->mLBIdxX + 1, y, eTileType::WallSW);
					}
					for (int y = closeRoom->mRTIdxY + 1; y <= neighbor->mRTIdxY; ++y)
					{
						mOwner->SetTileType(closeRoom->mRTIdxX - 2, y, eTileType::WallSW);
					}
					for (int y = closeRoom->mRTIdxY + 1; y < neighbor->mRTIdxY + 1; ++y)
					{
						mOwner->SetTileType(closeRoom->mRTIdxX - 1, y, eTileType::WallNE);
					}
					
					// 통로 교차점
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, mLBIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, neighbor->mRTIdxY - 1, eTileType::WallEndNWTop);

					// 통로가 진입점과 만나는 부분
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, mostBottom->mLBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(mostBottom->mLBIdxX + 2, mostBottom->mLBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, closeRoom->mRTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(closeRoom->mRTIdxX - 2, closeRoom->mRTIdxY + 1, eTileType::WallEndSEBottom);

					// 내부 코너
					mOwner->SetTileType(mostBottom->mLBIdxX + 1, mLBIdxY, eTileType::InnerCornerW);
					mOwner->SetTileType(closeRoom->mRTIdxX - 1, neighbor->mRTIdxY, eTileType::InnerCornerE);
				}
			}
		}
	}
}

CD2Room* CD2Space::GetLeftMostRoomInfo()
{
	if (mRoom)
	{
		return mRoom;
	}

	CD2Room* room1 = mChild1->GetLeftMostRoomInfo();
	CD2Room* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->mLBIdxX < room2->mLBIdxX)
	{
		return room1;
	}

	return room2;
}

CD2Room* CD2Space::GetRightMostRoomInfo()
{
	if (mRoom)
	{
		return mRoom;
	}

	CD2Room* room1 = mChild1->GetRightMostRoomInfo();
	CD2Room* room2 = mChild2->GetRightMostRoomInfo();

	if (room1->mRTIdxX > room2->mRTIdxX)
	{
		return room1;
	}

	return room2;
}

CD2Room* CD2Space::GetTopMostRoomInfo()
{
	if (mRoom)
	{
		return mRoom;
	}

	CD2Room* room1 = mChild1->GetTopMostRoomInfo();
	CD2Room* room2 = mChild2->GetTopMostRoomInfo();

	if (room1->mRTIdxY > room2->mRTIdxY)
	{
		return room1;
	}

	return room2;
}

CD2Room* CD2Space::GetBottomMostRoomInfo()
{
	if (mRoom)
	{
		return mRoom;
	}

	CD2Room* room1 = mChild1->GetBottomMostRoomInfo();
	CD2Room* room2 = mChild2->GetBottomMostRoomInfo();

	if (room1->mLBIdxY < room2->mLBIdxY)
	{
		return room1;
	}

	return room2;
}

CD2Room* CD2Space::GetMostCloseRoomInfo(const int centerIdx)
{
	if (mRoom)
	{
		return mRoom;
	}

	CD2Room* room1 = mChild1->GetMostCloseRoomInfo(centerIdx);
	CD2Room* room2 = mChild2->GetMostCloseRoomInfo(centerIdx);

	int centerX = centerIdx % mOwner->GetMapCountX();
	int centerY = centerIdx / mOwner->GetMapCountX();
	int x1 = room1->mCenterIdx % mOwner->GetMapCountX();
	int y1 = room1->mCenterIdx / mOwner->GetMapCountX();
	int x2 = room2->mCenterIdx % mOwner->GetMapCountX();
	int y2 = room2->mCenterIdx / mOwner->GetMapCountX();

	// 거리 비교	
	int len1 = abs(centerX - x1) + abs(centerY - y1);
	int len2 = abs(centerX - x2) + abs(centerY - y2);

	if (len1 < len2)
	{
		return room1;
	}
	return room2;
}

void CD2Space::doPartitioning(const int maxLevel)
{	
	if (mParent)
	{
		mLevel = mParent->mLevel + 1;
	}

	// 레벨별로 등록
	mOwner->AddSpace(this, mLevel);

	// 최대 분할 레벨에 도달한 경우 분할 중단
	if (maxLevel == mLevel)
	{
		return;
	}

	// 최소한의 공간 사이즈 ( 최소 방 사이즈 + 2 )
	int minSpaceSize = mOwner->GetRoomSizeMin() + 2;

	// 이 공간에 최소 사이즈 공간 두개가 들어갈 수 없다면, 분할을 중단.
	if (mSizeX < (( mOwner->GetRoomSizeMin() + 2) * 2) || mSizeY < ((mOwner->GetRoomSizeMin() + 2) * 2))
	{
		return;
	}

	mChild1 = new CD2Space;
	mChild2 = new CD2Space;

	mChild1->mParent = this;
	mChild2->mParent = this;

	mChild1->mOwner = mOwner;
	mChild2->mOwner = mOwner;

	mChild1->Init();
	mChild2->Init();

	eSplitDir dir;

	if (mSizeX <= mSizeY)
	{
		dir = eSplitDir::Horizontal;
	}
	else
	{
		dir = eSplitDir::Vertical;
	}

	mChild1->meSplitDir = dir;
	mChild2->meSplitDir = dir;

	// 4 : 6 ~ 6 : 4의 비율로 크기 분배
	float ratio = (4.f + (float)(rand() % 3)) / 10.f;

	// 4 : 6 으로 분할했을 때, 작은 사이즈 공간이 최소 공간 사이즈보다 작을 경우, 5 : 5로만 분할한다.
	if (((mSizeX / 2 * 0.4f) <= minSpaceSize) || ((mSizeY / 2 * 0.4f) <= minSpaceSize))
	{
		ratio = 0.5f;
	}
	
	// 분할
	if (eSplitDir::Horizontal == dir)
	{
		// child1 이 아래쪽 
		mChild1->mLBIdxX = mLBIdxX;
		mChild1->mLBIdxY = mLBIdxY;

		mChild1->mRTIdxX = mRTIdxX;
		mChild1->mRTIdxY = mLBIdxY + (mSizeY * ratio) - 1;

		mChild2->mLBIdxX = mLBIdxX;
		mChild2->mLBIdxY = mLBIdxY + (mSizeY * ratio);

		mChild2->mRTIdxX = mRTIdxX;
		mChild2->mRTIdxY = mRTIdxY;
	}
	else
	{
		// child1이 왼쪽
		mChild1->mLBIdxX = mLBIdxX;
		mChild1->mLBIdxY = mLBIdxY;

		mChild1->mRTIdxX = mLBIdxX + (mSizeX * ratio) - 1;
		mChild1->mRTIdxY = mRTIdxY;

		mChild2->mLBIdxX = mLBIdxX + (mSizeX * ratio);
		mChild2->mLBIdxY = mLBIdxY;

		mChild2->mRTIdxX = mRTIdxX;
		mChild2->mRTIdxY = mRTIdxY;
	}

	// 사이즈 계산
	mChild1->mSizeX = mChild1->mRTIdxX - mChild1->mLBIdxX + 1;
	mChild1->mSizeY = mChild1->mRTIdxY - mChild1->mLBIdxY + 1;

	mChild2->mSizeX = mChild2->mRTIdxX - mChild2->mLBIdxX + 1;
	mChild2->mSizeY = mChild2->mRTIdxY - mChild2->mLBIdxY + 1;

	// 나머지 인덱스 계산
	mChild1->mLBIdx = mChild1->mLBIdxY * mOwner->GetMapCountX() + mChild1->mLBIdxX;
	mChild1->mRTIdx = mChild1->mRTIdxY * mOwner->GetMapCountX() + mChild1->mRTIdxX;
	mChild1->mCenterIdxX = mChild1->mLBIdxX + (mChild1->mSizeX / 2);
	mChild1->mCenterIdxY = mChild1->mLBIdxY + (mChild1->mSizeY / 2);
	mChild1->mCenterIdx = mChild1->mCenterIdxY * mOwner->GetMapCountX() + mChild1->mCenterIdxX;

	mChild2->mLBIdx = mChild2->mLBIdxY * mOwner->GetMapCountX() + mChild2->mLBIdxX;
	mChild2->mRTIdx = mChild2->mRTIdxY * mOwner->GetMapCountX() + mChild2->mRTIdxX;
	mChild2->mCenterIdxX = mChild2->mLBIdxX + (mChild2->mSizeX / 2);
	mChild2->mCenterIdxY = mChild2->mLBIdxY + (mChild2->mSizeY / 2);
	mChild2->mCenterIdx = mChild2->mCenterIdxY * mOwner->GetMapCountX() + mChild2->mCenterIdxX;

	// 자식 공간 분할
	mChild1->doPartitioning(maxLevel);
	mChild2->doPartitioning(maxLevel);
}
