#include "Space.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/SpaceConstantBuffer.h"
#include "CameraComponent.h"
#include "../Engine.h"

CSpace::CSpace()	:
	mOwner(nullptr),
	mParent(nullptr),
	mChild1(nullptr),
	mChild2(nullptr),
	meSplitDir(eSplitDir::None),
	mRoomInfo(nullptr),
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

CSpace::~CSpace()
{
	SAFE_DELETE(mRoomInfo);
	SAFE_DELETE(mCBuffer);
}

bool CSpace::Init()
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

void CSpace::Render()
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

		CSpace* neighbor = GetNeighbor();

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

void CSpace::StartPartitioning(const int sizeX, const int sizeY, const int maxLevel)
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
	
	// ���ҽ���
	doPartitioning(maxLevel);
}

bool CSpace::MakeRoom()
{
	if (mChild1)
	{
		mChild1->MakeRoom();
	}
	if (mChild2)
	{
		mChild2->MakeRoom();
	}
	// �� ������ �� �̻� ���ҵ��� ���� �����̸� �� �����Ѵ�.
	else
	{
		mRoomInfo = new RoomInfo;
		mRoomInfo->Space = this;

		// �� ������ ���� ���� ��迡�� 2ĭ�� ������ ��
		int minIdxX = mLBIdxX + 2;
		int minIdxY = mLBIdxY + 2;
		int maxIdxX = mRTIdxX - 2;
		int maxIdxY = mRTIdxY - 2;

		if (maxIdxX - minIdxX + 1 <= mOwner->GetRoomSizeMin())
		{
			mRoomInfo->LBIdxX = minIdxX;
			mRoomInfo->SizeX = mOwner->GetRoomSizeMin();
			mRoomInfo->RTIdxX = minIdxX + mOwner->GetRoomSizeMin() - 1;
		}
		else
		{
			int maxLBX = maxIdxX - mOwner->GetRoomSizeMin();
			int range = (maxLBX - minIdxX + 1) % 5 + 1;
			mRoomInfo->LBIdxX = minIdxX + floor(rand() % range);

			int minRTX = mRoomInfo->LBIdxX + mOwner->GetRoomSizeMin() - 1;
			range = (maxIdxX - minRTX) % 5 + 1;
			mRoomInfo->RTIdxX = maxIdxX - floor(rand() % range);

			mRoomInfo->SizeX = mRoomInfo->RTIdxX - mRoomInfo->LBIdxX + 1;
		}

		if (maxIdxY - minIdxY + 1 <= mOwner->GetRoomSizeMin())
		{
			mRoomInfo->LBIdxY = minIdxY;
			mRoomInfo->SizeY = mOwner->GetRoomSizeMin();
			mRoomInfo->RTIdxY = minIdxY + mOwner->GetRoomSizeMin() - 1;
		}
		else
		{
			int maxLBY = maxIdxY - mOwner->GetRoomSizeMin();
			int range = (maxLBY - minIdxY + 1) % 5 + 1;
			mRoomInfo->LBIdxY = minIdxY + floor(rand() % range);

			int minRTY = mRoomInfo->LBIdxY + mOwner->GetRoomSizeMin() - 1;
			range = (maxIdxY - minRTY) % 5 + 1;
			mRoomInfo->RTIdxY = maxIdxY - floor(rand() % range);

			mRoomInfo->SizeY = mRoomInfo->RTIdxY - mRoomInfo->LBIdxY + 1;
		}

		mRoomInfo->RTIdx = mRoomInfo->RTIdxY * mOwner->GetMapCountX() + mRoomInfo->RTIdxX;
		mRoomInfo->LBIdx = mRoomInfo->LBIdxY * mOwner->GetMapCountX() + mRoomInfo->LBIdxX;

		if (mRoomInfo->RTIdxX >= mOwner->GetMapCountX() || mRoomInfo->RTIdxY >= mOwner->GetMapCountY() ||
			mRoomInfo->LBIdxX >= mOwner->GetMapCountX() || mRoomInfo->LBIdxY >= mOwner->GetMapCountY())
		{
			assert(false);
		}

		// �߾� �ε���
		mRoomInfo->CenterIdxX = mRoomInfo->LBIdxX + (mRoomInfo->SizeX / 2);
		mRoomInfo->CenterIdxY = mRoomInfo->LBIdxY + (mRoomInfo->SizeY / 2);
		mRoomInfo->CenterIdx = mRoomInfo->CenterIdxY * mOwner->GetMapCountX() + mRoomInfo->CenterIdxX;

		mOwner->AddRoom(mRoomInfo);
	}
	return true;
}

void CSpace::ConnectSpace()
{
	// �ֻ��� ������ ���
	if (!mParent)
	{
		return;
	}

	CSpace* neighbor = GetNeighbor();
	mbConnect = true;
	neighbor->mbConnect = true;

	// �� ������ ���η� �������� ������ ���� ���
	if (eSplitDir::Horizontal == meSplitDir)
	{
		// �� ������ ���� ������ ���
		if (mCenterIdxX < neighbor->mCenterIdxX)
		{
			// �� ������ ���� �����ʿ� �ִ� ���, �� �濡�� ���� ����� �̿� ������ ���� ã�´�. 
			RoomInfo* mostRight = GetRightMostRoomInfo();
			RoomInfo* closeRoom = neighbor->GetMostCloseRoomInfo(mostRight->CenterIdx);

			// ��θ� ��� �̾�� �ϴ� ���
			// 1. �� ������ ���� �̾�� �� �溸�� ���� �ִ� ���
			if (mostRight->RTIdxY - closeRoom->LBIdxY < 3)
			{
				// ��� ����
				for (int y = mostRight->RTIdxY - 1; y <= closeRoom->LBIdxY + 1; ++y)
				{
					mOwner->SetTileType(mRTIdxX, y, eTileType::WallSW);
					mOwner->SetTileType(neighbor->mLBIdxX, y, eTileType::WallNE);
				}

				// �ڳ� ��ġ
				mOwner->SetTileType(mRTIdxX, closeRoom->LBIdxY - 2, eTileType::CornerW);
				mOwner->SetTileType(neighbor->mLBIdxX, mostRight->RTIdxY - 2, eTileType::CornerE);

				// ������ ����
				mOwner->SetTileType(mostRight->RTIdxX, mostRight->RTIdxY - 2, eTileType::EntryNERight);
				mOwner->SetTileType(mostRight->RTIdxX, mostRight->RTIdxY - 1, eTileType::EntryNELeft);
				mOwner->SetTileType(closeRoom->LBIdxX, closeRoom->LBIdxY + 2, eTileType::EntrySWLeft);
				mOwner->SetTileType(closeRoom->LBIdxX, closeRoom->LBIdxY + 1, eTileType::EntrySWRight);

				// �濡�� �ٷ� �̾����� ���
				for (int x = mostRight->RTIdxX + 1; x <= mRTIdxX; ++x)
				{
					mOwner->SetTileType(x, mostRight->RTIdxY - 2, eTileType::WallSE);
				}
				for (int x = mostRight->RTIdxX + 1; x < mRTIdxX - 1; ++x)
				{
					mOwner->SetTileType(x, mostRight->RTIdxY - 1, eTileType::WallNW);
				}
				for (int x = closeRoom->LBIdxX - 1; x >= neighbor->mLBIdxX; --x)
				{
					mOwner->SetTileType(x, closeRoom->LBIdxY + 2, eTileType::WallNW);
				}
				for (int x = closeRoom->LBIdxX - 1; x > neighbor->mLBIdxX + 1; --x)
				{
					mOwner->SetTileType(x, closeRoom->LBIdxY + 1, eTileType::WallSE);
				}

				// ��� ������
				mOwner->SetTileType(mRTIdxX - 1, mostRight->RTIdxY - 1, eTileType::WallEndNWTop);
				mOwner->SetTileType(neighbor->mLBIdxX + 1, closeRoom->LBIdxY + 1, eTileType::WallEndSEBottom);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->RTIdxY - 2, eTileType::WallEndSWTop);
				mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->RTIdxY - 1, eTileType::WallEndSWBottom);
				mOwner->SetTileType(closeRoom->LBIdxX - 1, closeRoom->LBIdxY + 2, eTileType::WallEndNETop);
				mOwner->SetTileType(closeRoom->LBIdxX - 1, closeRoom->LBIdxY + 1, eTileType::WallEndNEBottom);

				// ���� �ڳ�
				mOwner->SetTileType(mRTIdxX, mostRight->RTIdxX - 1, eTileType::InnerCornerW);
				mOwner->SetTileType(neighbor->mLBIdxX, closeRoom->LBIdxY + 1, eTileType::InnerCornerE);
			}
			// 2. �� ������ ���� �̾�� �� �溸�� ���� �ִ� ���
			else if (closeRoom->LBIdxY - mostRight->RTIdxY < 3)
			{
				// 1. ��� ����
				for (int y = mostRight->LBIdxY + 1; y >= closeRoom->RTIdxY - 1; --y)
				{
					mOwner->SetTileType(mRTIdxX, y, eTileType::WallSW);
					mOwner->SetTileType(neighbor->mLBIdxX, y, eTileType::WallNE);
				}

				// 2 ��� �ڳ� ��ġ
				mOwner->SetTileType(neighbor->mLBIdxX, mostRight->LBIdxY + 2, eTileType::CornerN);
				mOwner->SetTileType(mRTIdxX, closeRoom->RTIdxY - 2, eTileType::CornerS);

				// 3. ������ ����
				mOwner->SetTileType(mostRight->RTIdxX, mostRight->LBIdxY - 2, eTileType::EntryNELeft);
				mOwner->SetTileType(mostRight->RTIdxX, mostRight->LBIdxY - 1, eTileType::EntryNERight);
				mOwner->SetTileType(closeRoom->LBIdxX, closeRoom->RTIdxY - 2, eTileType::EntrySWRight);
				mOwner->SetTileType(closeRoom->LBIdxX, closeRoom->RTIdxY - 1, eTileType::EntrySWLeft);

				// 4. �濡�� �ٷ� �̾����� ���
				for (int x = mostRight->RTIdxX + 1; x <= mRTIdxX; ++x)
				{
					mOwner->SetTileType(x, mostRight->LBIdxY - 2, eTileType::WallNW);
				}
				for (int x = mostRight->RTIdxX + 1; x < mRTIdxX - 1; ++x)
				{
					mOwner->SetTileType(x, mostRight->LBIdxY - 1, eTileType::WallSE);
				}
				for (int x = closeRoom->LBIdxX - 1; x >= neighbor->mLBIdxX; --x)
				{
					mOwner->SetTileType(x, closeRoom->RTIdxY - 2, eTileType::WallSE);
				}
				for (int x = closeRoom->LBIdxX - 1; x > neighbor->mLBIdxX; --x)
				{
					mOwner->SetTileType(x, closeRoom->RTIdxY - 1, eTileType::WallNW);
				}
				
				// 5. ��� ������
				mOwner->SetTileType(mRTIdxX - 1, mostRight->LBIdxY + 1, eTileType::WallEndNEBottom);
				mOwner->SetTileType(neighbor->mLBIdxX - 1, closeRoom->RTIdxY - 1, eTileType::WallEndSWTop);

				// 6. ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->LBIdxY + 2, eTileType::WallEndSWTop);
				mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->LBIdxY + 1, eTileType::WallEndSWBottom);
				mOwner->SetTileType(closeRoom->LBIdxX - 1, closeRoom->RTIdxY - 2, eTileType::WallEndNEBottom);
				mOwner->SetTileType(closeRoom->LBIdxX - 1, closeRoom->RTIdxY - 1, eTileType::WallEndNETop);

				// 7. ���� �ڳ�
				mOwner->SetTileType(mRTIdxX, mostRight->LBIdxY + 1, eTileType::InnerCornerS);
				mOwner->SetTileType(neighbor->mLBIdxX, closeRoom->RTIdxY - 1, eTileType::InnerCornerN);
			}
			// ���� ��η� �̾ �Ǵ� ���
			else
			{
				bool bTopEntry = rand() > RAND_MAX / 2;

				// �� ������ ���ʿ��� ��θ� �����Ѵ�.
				if (bTopEntry)
				{
					// ��� ����
					for (int x = mostRight->RTIdxX + 1; x <= closeRoom->LBIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mostRight->RTIdxY - 1, eTileType::WallNW);
						mOwner->SetTileType(x, mostRight->RTIdxY - 2, eTileType::WallSE);
					}
					
					// ������ ��ġ
					mOwner->SetTileType(mostRight->RTIdxX, mostRight->RTIdxY - 2, eTileType::EntryNELeft);
					mOwner->SetTileType(mostRight->RTIdxX, mostRight->RTIdxY - 1, eTileType::EntryNERight);
					mOwner->SetTileType(closeRoom->LBIdxX, mostRight->RTIdxY - 2, eTileType::EntrySWLeft);
					mOwner->SetTileType(closeRoom->LBIdxX, mostRight->RTIdxY - 1, eTileType::EntrySWRight);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->RTIdxY - 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->RTIdxY - 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(closeRoom->LBIdxX - 1, mostRight->RTIdxY - 2, eTileType::WallEndNETop);
					mOwner->SetTileType(closeRoom->LBIdxX - 1, mostRight->RTIdxY - 1, eTileType::WallEndNEBottom);
				}
				// �� ������ �Ʒ��ʿ��� ��θ� �����Ѵ�.
				else
				{
					for (int x = mostRight->RTIdxX + 1; x <= closeRoom->LBIdxX - 1; ++x)
					{
						mOwner->SetTileType(x, mostRight->LBIdxY + 1, eTileType::WallSE);
						mOwner->SetTileType(x, mostRight->LBIdxY + 2, eTileType::WallNW);
					}

					// ������ ��ġ
					mOwner->SetTileType(mostRight->RTIdxX, mostRight->LBIdxY + 1, eTileType::EntryNERight);
					mOwner->SetTileType(mostRight->RTIdxX, mostRight->LBIdxY + 2, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->LBIdxX, mostRight->LBIdxY + 1, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->LBIdxX, mostRight->LBIdxY + 2, eTileType::EntrySWLeft);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->LBIdxY + 1, eTileType::WallEndSWBottom);
					mOwner->SetTileType(mostRight->RTIdxX + 1, mostRight->LBIdxY + 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->LBIdxX - 1, mostRight->LBIdxY + 1, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->LBIdxX - 1, mostRight->LBIdxY + 2, eTileType::WallEndNETop);
				}
			}
		}
		// �������� ���
		else
		{
			RoomInfo* mostLeft = GetLeftMostRoomInfo();
			RoomInfo* closeRoom = neighbor->GetMostCloseRoomInfo(mostLeft->CenterIdx);

			// ��θ� ��� �̾�� �ϴ� ���
			// 1. �� ������ ���� �̾�� �� �溸�� ���� �ִ� ���
			if (mostLeft->RTIdxY - closeRoom->LBIdxY < 3)
			{
				// ��� ����
				for (int y = mostLeft->RTIdxY - 1; y <= closeRoom->LBIdxY + 1; ++y)
				{
					mOwner->SetTileType(mLBIdxX, y, eTileType::WallNE);
					mOwner->SetTileType(neighbor->mRTIdxX, y, eTileType::WallSW);
				}

				// ��� �ڳ� ��ġ
				mOwner->SetTileType(neighbor->mRTIdxX, mostLeft->RTIdxY - 2, eTileType::CornerS);
				mOwner->SetTileType(mLBIdxX, closeRoom->LBIdxY + 2, eTileType::CornerN);

				// ������ ����
				mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->RTIdxY - 1, eTileType::EntrySWLeft);
				mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->RTIdxY - 2, eTileType::EntrySWRight);
				mOwner->SetTileType(closeRoom->RTIdxX, closeRoom->LBIdxY + 1, eTileType::EntryNERight);
				mOwner->SetTileType(closeRoom->RTIdxX, closeRoom->LBIdxY + 2, eTileType::EntryNELeft);

				// �濡�� �ٷ� �̾����� ���
				for (int x = mostLeft->LBIdxX - 1; x >= mLBIdxX; --x)
				{
					mOwner->SetTileType(x, mostLeft->RTIdxY - 2, eTileType::WallSE);
				}
				for (int x = mostLeft->LBIdxX - 1; x > mLBIdxX + 1; --x)
				{
					mOwner->SetTileType(x, mostLeft->RTIdxY - 1, eTileType::WallNW);
				}
				for (int x = closeRoom->RTIdxX + 1; x <= neighbor->mRTIdxX; ++x)
				{
					mOwner->SetTileType(x, closeRoom->LBIdxY + 2, eTileType::WallNW);
				}
				for (int x = closeRoom->RTIdxX + 1; x < neighbor->mRTIdxX - 1; ++x)
				{
					mOwner->SetTileType(x, closeRoom->LBIdxY + 1, eTileType::WallSE);
				}
				
				// ��� ������
				mOwner->SetTileType(mLBIdxX + 1, mostLeft->RTIdxY - 1, eTileType::WallEndSWTop);
				mOwner->SetTileType(neighbor->mRTIdxX - 1, closeRoom->LBIdxX + 1, eTileType::WallEndNEBottom);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->RTIdxY - 1, eTileType::WallEndNETop);
				mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->RTIdxY - 2, eTileType::WallEndNEBottom);
				mOwner->SetTileType(closeRoom->RTIdxX + 1, closeRoom->LBIdxY + 1, eTileType::WallEndSWBottom);
				mOwner->SetTileType(closeRoom->RTIdxX + 1, closeRoom->LBIdxY + 2, eTileType::WallEndSWTop);
				
				// ���� �ڳ�
				mOwner->SetTileType(mLBIdxX, mostLeft->RTIdxY - 1, eTileType::InnerCornerN);
				mOwner->SetTileType(neighbor->mRTIdxX, closeRoom->LBIdxX + 1, eTileType::InnerCornerS);
			}
			// 2. �� ������ ���� �̾�� �� �溸�� ���� �ִ� ���
			else if (closeRoom->RTIdxY - mostLeft->LBIdxY < 3)
			{
				// ��� ����
				for (int y = mostLeft->LBIdxY + 1; y >= closeRoom->RTIdxY - 1; --y)
				{
					mOwner->SetTileType(mLBIdxX, y, eTileType::WallNE);
					mOwner->SetTileType(neighbor->mRTIdxX, eTileType::WallSW);
				}

				// ��� �ڳ� ��ġ
				mOwner->SetTileType(mLBIdxX, closeRoom->RTIdxY - 2, eTileType::CornerE);
				mOwner->SetTileType(neighbor->mRTIdxX, mostLeft->LBIdxY + 2, eTileType::CornerW);

				// ������ ����
				mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->LBIdxX + 1, eTileType::EntrySWRight);
				mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->LBIdxX + 2, eTileType::EntrySWLeft);
				mOwner->SetTileType(closeRoom->RTIdxX, closeRoom->RTIdxY - 1, eTileType::EntryNELeft);
				mOwner->SetTileType(closeRoom->RTIdxX, closeRoom->RTIdxY - 2, eTileType::EntryNERight);

				// �濡�� �ٷ� �̾����� ���
				for (int x = mostLeft->LBIdxX - 1; x >= mLBIdxX; --x)
				{
					mOwner->SetTileType(x, mostLeft->LBIdxY + 2, eTileType::WallNW);
				}
				for (int x = mostLeft->LBIdxX - 1; x > mLBIdxX - 1; --x)
				{
					mOwner->SetTileType(x, mostLeft->LBIdxY + 1, eTileType::WallSE);
				}
				for (int x = closeRoom->RTIdxX + 1; x <= neighbor->mRTIdxX; ++x)
				{
					mOwner->SetTileType(x, closeRoom->RTIdxY - 2, eTileType::WallSE);
				}
				for (int x = closeRoom->RTIdxX + 1; x < neighbor->mRTIdxX - 1; ++x)
				{
					mOwner->SetTileType(x, closeRoom->RTIdxY - 1, eTileType::WallNW);
				}
				
				// ��� ������
				mOwner->SetTileType(mLBIdxX + 1, mostLeft->LBIdxY + 1, eTileType::WallEndSWBottom);
				mOwner->SetTileType(neighbor->mRTIdxX - 1, closeRoom->RTIdxY - 1, eTileType::WallEndNETop);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->LBIdxY + 2, eTileType::WallEndNETop);
				mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->LBIdxY + 1, eTileType::WallEndNEBottom);
				mOwner->SetTileType(closeRoom->RTIdxX + 1, closeRoom->RTIdxY - 1, eTileType::WallEndSWTop);
				mOwner->SetTileType(closeRoom->RTIdxX + 1, closeRoom->RTIdxY - 2, eTileType::WallEndSWBottom);

				// ���� �ڳ�
				mOwner->SetTileType(mLBIdxX, mostLeft->LBIdxY + 1, eTileType::InnerCornerE);
				mOwner->SetTileType(neighbor->mRTIdxX, closeRoom->RTIdxY - 1, eTileType::InnerCornerW);
			}
			// ���� ��η� �̾ �Ǵ� ���
			else
			{
				bool bTopEntry = rand() > RAND_MAX / 2;

				// �� ������ ���ʿ��� ��θ� ����
				if (bTopEntry)
				{
					// ��� ����
					for (int x = mostLeft->LBIdxX - 1; x >= closeRoom->RTIdxX + 1; --x)
					{
						mOwner->SetTileType(x, mostLeft->RTIdxY - 1, eTileType::WallNW);
						mOwner->SetTileType(x, mostLeft->RTIdxY - 2, eTileType::WallSE);
					}
					
					// ������ ��ġ
					mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->RTIdxY - 1, eTileType::EntrySWLeft);
					mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->RTIdxY - 2, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->RTIdxX, mostLeft->RTIdxY - 1, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->RTIdxX, mostLeft->RTIdxY - 2, eTileType::EntryNERight);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->RTIdxY - 1, eTileType::WallEndNETop);
					mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->RTIdxY - 2, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->RTIdxX + 1, mostLeft->RTIdxY - 1, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->RTIdxX + 1, mostLeft->RTIdxY - 2, eTileType::WallEndSWBottom);
				}
				// �� ������ �Ʒ��ʿ��� ��θ� ����
				else
				{
					for (int x = mostLeft->LBIdxX; x >= closeRoom->RTIdxX + 1; --x)
					{
						mOwner->SetTileType(x, mostLeft->LBIdxY + 2, eTileType::WallNW);
						mOwner->SetTileType(x, mostLeft->LBIdxY + 1, eTileType::WallSE);
					}

					// ������ ��ġ
					mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->LBIdxY + 2, eTileType::EntrySWLeft);
					mOwner->SetTileType(mostLeft->LBIdxX, mostLeft->LBIdxY + 1, eTileType::EntrySWRight);
					mOwner->SetTileType(closeRoom->RTIdxX, mostLeft->LBIdxY + 2, eTileType::EntryNELeft);
					mOwner->SetTileType(closeRoom->RTIdxX, mostLeft->LBIdxY + 1, eTileType::EntryNERight);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->LBIdxY + 2, eTileType::WallEndNETop);
					mOwner->SetTileType(mostLeft->LBIdxX - 1, mostLeft->LBIdxY + 1, eTileType::WallEndNEBottom);
					mOwner->SetTileType(closeRoom->RTIdxX + 1, mostLeft->LBIdxY + 2, eTileType::WallEndSWTop);
					mOwner->SetTileType(closeRoom->RTIdxX + 1, mostLeft->LBIdxY + 1, eTileType::WallEndSWBottom);
				}
			}
		}
	}
	// �� ������ ���η� �������� ������ ���� ���
	else
	{
		// �� ������ �Ʒ��� ������ ���
		if (mCenterIdxY < neighbor->mCenterIdxY)
		{
			RoomInfo* mostTop = GetTopMostRoomInfo();
			RoomInfo* closeRoom = neighbor->GetMostCloseRoomInfo(mostTop->CenterIdx);

			// ��θ� ��� �̾�� �� ���
			// 1. �� ������ ���� �̿� ������ �溸�� ���ʿ� �ִ� ���
			if (mostTop->RTIdxX - closeRoom->LBIdxX < 3)
			{
				// ��� ����
				for (int x = mostTop->RTIdxX - 1; x <= closeRoom->LBIdxX - 1; ++x)
				{
					mOwner->SetTileType(x, mRTIdxY, eTileType::WallSE);
					mOwner->SetTileType(x, neighbor->mLBIdxY, eTileType::WallNW);
				}

				// ��� �ڳ� ��ġ
				mOwner->SetTileType(mostTop->RTIdxX - 2, neighbor->mLBIdxX, eTileType::CornerW);
				mOwner->SetTileType(closeRoom->LBIdxX + 2, mRTIdxY, eTileType::CornerE);

				// ������ ����
				mOwner->SetTileType(mostTop->RTIdxX - 2, mostTop->RTIdxY, eTileType::EntryNWLeft);
				mOwner->SetTileType(mostTop->RTIdxX - 1, mostTop->RTIdxY, eTileType::EntryNWRight);
				mOwner->SetTileType(closeRoom->LBIdxX - 1, closeRoom->LBIdxY, eTileType::EntrySELeft);
				mOwner->SetTileType(closeRoom->LBIdxX - 2, closeRoom->LBIdxY, eTileType::EntrySERight);

				// �濡�� �ٷ� �̾����� ���
				for (int y = mostTop->RTIdxY + 1; y <= mRTIdxY; ++y)
				{
					mOwner->SetTileType(mostTop->RTIdxX - 2, y, eTileType::WallSW);
				}
				for (int y = mostTop->RTIdxY + 1; y < mRTIdxY - 1; ++y)
				{
					mOwner->SetTileType(mostTop->RTIdxX - 1, y, eTileType::WallNE);
				}
				for (int y = closeRoom->LBIdxY - 1; y >= neighbor->mLBIdxY; --y)
				{
					mOwner->SetTileType(closeRoom->LBIdxX + 2, eTileType::WallNE);
				}
				for (int y = closeRoom->LBIdxY - 1; y > neighbor->mLBIdxY + 1; --y)
				{
					mOwner->SetTileType(closeRoom->LBIdxX + 1, eTileType::WallSW);
				}
				
				// ��� ������
				mOwner->SetTileType(mostTop->RTIdxX - 1, mRTIdxY - 1, eTileType::WallEndNETop);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, neighbor->mLBIdxY - 1, eTileType::WallEndSEBottom);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostTop->RTIdxX - 1, mostTop->RTIdxY + 1, eTileType::WallEndSETop);
				mOwner->SetTileType(mostTop->RTIdxX - 2, mostTop->RTIdxY + 1, eTileType::WallEndSEBottom);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, closeRoom->LBIdxY - 1, eTileType::WallEndNWBottom);
				mOwner->SetTileType(closeRoom->LBIdxX + 2, closeRoom->LBIdxY - 1, eTileType::WallEndNWTop);

				// ���� �ڳ�
				mOwner->SetTileType(mostTop->RTIdxX - 1, mRTIdxY, eTileType::InnerCornerE);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, neighbor->mLBIdxY, eTileType::InnerCornerW);
			}

			// 2. �� ������ ���� �̿� ������ �溸�� �����ʿ� �ִ� ���
			if (closeRoom->RTIdxX - mostTop->LBIdxX < 3)
			{	
				// ��� ����
				for (int x = closeRoom->RTIdxX - 1; x <= mostTop->LBIdxX + 1; ++x)
				{
					mOwner->SetTileType(x, neighbor->mLBIdxY, eTileType::WallNW);
					mOwner->SetTileType(x, mRTIdxY, eTileType::WallSE);
				}

				// ��� �ڳ� ��ġ
				mOwner->SetTileType(mostTop->LBIdxX + 2, neighbor->mLBIdxY, eTileType::CornerN);
				mOwner->SetTileType(closeRoom->RTIdxX - 2, mLBIdxY, eTileType::CornerS);

				// ������ ����
				mOwner->SetTileType(mostTop->LBIdxX + 1, mostTop->RTIdxY, eTileType::EntryNWLeft);
				mOwner->SetTileType(mostTop->LBIdxX + 2, mostTop->RTIdxY, eTileType::EntryNWRight);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, closeRoom->LBIdxY, eTileType::EntrySERight);
				mOwner->SetTileType(closeRoom->RTIdxX - 2, closeRoom->LBIdxY, eTileType::EntrySELeft);

				// �濡�� �ٷ� �̾����� ���
				for (int y = mostTop->RTIdxY + 1; y <= mRTIdxY; ++y)
				{
					mOwner->SetTileType(mostTop->LBIdxX + 2, y, eTileType::WallNE);
				}
				for (int y = mostTop->RTIdxY + 1; y < mRTIdxY - 1; ++y)
				{
					mOwner->SetTileType(mostTop->LBIdxX + 1, y, eTileType::WallSW);
				}
				for (int y = closeRoom->LBIdxY - 1; y >= neighbor->mLBIdxY; --y)
				{
					mOwner->SetTileType(closeRoom->RTIdxX - 2, y, eTileType::WallSW);
				}
				for (int y = closeRoom->LBIdxY - 1; y > neighbor->mLBIdxY + 1; --y)
				{
					mOwner->SetTileType(closeRoom->RTIdxX - 1, y, eTileType::WallNE);
				}
				
				// ��� ������
				mOwner->SetTileType(mostTop->LBIdxX + 1, mRTIdxY - 1, eTileType::WallEndSWBottom);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, neighbor->mLBIdxY + 1, eTileType::WallEndNETop);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostTop->LBIdxX + 2, mostTop->RTIdxY + 1, eTileType::WallEndSETop);
				mOwner->SetTileType(mostTop->LBIdxX + 1, mostTop->RTIdxY + 1, eTileType::WallEndSEBottom);
				mOwner->SetTileType(closeRoom->RTIdxX - 2, closeRoom->LBIdxY - 1, eTileType::WallEndNWBottom);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, closeRoom->LBIdxY - 1, eTileType::WallEndNWTop);

				// ���� �ڳ�
				mOwner->SetTileType(mostTop->LBIdxX + 1, mRTIdxY, eTileType::InnerCornerS);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, neighbor->mLBIdxY, eTileType::InnerCornerN);
			}
			// 3. ���� ��η� �̾ �Ǵ� ���
			else
			{
				bool bLeftEntry = rand() > RAND_MAX / 2;
				
				// �� ���� ���� ���ʿ��� ��� ����
				if (bLeftEntry)
				{
					// ��� ����
					for (int y = mostTop->RTIdxY + 1; y <= closeRoom->LBIdxY - 1; ++y)
					{
						mOwner->SetTileType(mostTop->LBIdxX + 1, y, eTileType::WallSW);
						mOwner->SetTileType(mostTop->LBIdxX + 2, y, eTileType::WallNE);
					}
					
					// ������ ��ġ
					mOwner->SetTileType(mostTop->LBIdxX + 1, mostTop->RTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostTop->LBIdxX + 2, mostTop->RTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(mostTop->LBIdxX + 1, closeRoom->LBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostTop->LBIdxX + 2, closeRoom->LBIdxY, eTileType::EntrySERight);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostTop->LBIdxX + 2, mostTop->RTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostTop->LBIdxX + 1, mostTop->RTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostTop->LBIdxX + 2, closeRoom->LBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostTop->LBIdxX + 1, closeRoom->LBIdxY - 1, eTileType::WallEndNWBottom);
				}
				// �� ���� ���� �����ʿ��� ��� ����
				else
				{
					// ��� ����
					for (int y = mostTop->RTIdxY + 1; y <= closeRoom->LBIdxY - 1; ++y)
					{
						mOwner->SetTileType(mostTop->RTIdxX - 1, y, eTileType::WallNE);
						mOwner->SetTileType(mostTop->RTIdxX - 2, y, eTileType::WallNE);
					}

					// ������ ��ġ
					mOwner->SetTileType(mostTop->RTIdxX - 2, mostTop->RTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostTop->RTIdxX - 1, mostTop->RTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(mostTop->RTIdxX - 2, closeRoom->LBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostTop->RTIdxX - 1, closeRoom->LBIdxY, eTileType::EntrySERight);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostTop->RTIdxX - 1, mostTop->RTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostTop->RTIdxX - 2, mostTop->RTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostTop->RTIdxX - 1, closeRoom->LBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostTop->RTIdxX - 2, closeRoom->LBIdxY - 1, eTileType::WallEndNWBottom);
				}
			}
		}
		// �� ������ ���� ������ ���
		else
		{
			RoomInfo* mostBottom = GetBottomMostRoomInfo();
			RoomInfo* closeRoom = neighbor->GetMostCloseRoomInfo(mostBottom->CenterIdx);

			// �� ������ ���� �̿� ������ �溸�� ���ʿ� �ִ� ���
			if (mostBottom->RTIdxX - closeRoom->LBIdxX < 3)
			{
				// ��� ����
				for (int x = mostBottom->RTIdxX - 1; x <= closeRoom->LBIdxX + 1; ++x)
				{
					mOwner->SetTileType(x, mLBIdxY, eTileType::WallNW);
					mOwner->SetTileType(x, neighbor->mRTIdxY, eTileType::WallSE);
				}

				// ��� �ڳ� ��ġ
				mOwner->SetTileType(mostBottom->RTIdxX - 2, neighbor->mRTIdxY, eTileType::CornerS);
				mOwner->SetTileType(closeRoom->LBIdxX + 2, mLBIdxY, eTileType::CornerN);

				// ������ ����
				mOwner->SetTileType(mostBottom->RTIdxX - 1, mostBottom->LBIdxY, eTileType::EntrySERight);
				mOwner->SetTileType(mostBottom->RTIdxX - 2, mostBottom->LBIdxY, eTileType::EntrySELeft);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, closeRoom->RTIdxY, eTileType::EntryNELeft);
				mOwner->SetTileType(closeRoom->LBIdxX + 2, closeRoom->RTIdxY, eTileType::EntryNERight);

				// �濡�� �ٷ� �̾����� ���
				for (int y = mostBottom->LBIdxY - 1; y >= mLBIdxY; --y)
				{
					mOwner->SetTileType(mostBottom->RTIdxX - 2, y, eTileType::WallSW);
				}
				for (int y = mostBottom->LBIdxY - 1; y > mLBIdxY + 1; --y)
				{
					mOwner->SetTileType(mostBottom->RTIdxX - 1, y, eTileType::WallNE);
				}
				for (int y = closeRoom->RTIdxY + 1; y <= neighbor->mRTIdxY; ++y)
				{
					mOwner->SetTileType(closeRoom->LBIdxX + 2, y, eTileType::WallNE);
				}
				for (int y = closeRoom->RTIdxY + 1; y < neighbor->mRTIdxY - 1; ++y)
				{
					mOwner->SetTileType(closeRoom->LBIdxX + 1, y, eTileType::WallSW);
				}
				
				// ��� ������
				mOwner->SetTileType(mostBottom->RTIdxX - 1, mLBIdxY + 1, eTileType::WallEndNETop);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, neighbor->mRTIdxY - 1, eTileType::WallEndNWBottom);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostBottom->RTIdxX - 1, mostBottom->LBIdxY - 1, eTileType::WallEndNWTop);
				mOwner->SetTileType(mostBottom->RTIdxX - 2, mostBottom->LBIdxY - 1, eTileType::WallEndNWBottom);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, closeRoom->RTIdxY + 1, eTileType::WallEndSEBottom);
				mOwner->SetTileType(closeRoom->LBIdxX + 2, closeRoom->RTIdxY + 1, eTileType::WallEndSETop);

				// ���� �ڳ�
				mOwner->SetTileType(mostBottom->RTIdxX - 1, mLBIdxY, eTileType::InnerCornerE);
				mOwner->SetTileType(closeRoom->LBIdxX + 1, neighbor->mRTIdxY, eTileType::InnerCornerW);
			}

			// 2. �� ������ ���� �̿� ������ �溸�� �����ʿ� �ִ� ���
			if (closeRoom->RTIdxX - mostBottom->LBIdxX < 3)
			{	
				// ��� ����
				for (int x = mostBottom->LBIdxX + 1; x >= closeRoom->RTIdxX - 1; --x)
				{
					mOwner->SetTileType(x, mLBIdxY, eTileType::WallNW);
					mOwner->SetTileType(x, neighbor->mRTIdxY, eTileType::WallSE);
				}

				// ��� �ڳ� ��ġ
				mOwner->SetTileType(mostBottom->LBIdxX + 2, neighbor->mRTIdxY, eTileType::CornerE);
				mOwner->SetTileType(closeRoom->RTIdxX - 2, mLBIdxY, eTileType::CornerW);

				// ������ ����
				mOwner->SetTileType(mostBottom->LBIdxX + 1, mostBottom->LBIdxY, eTileType::EntrySELeft);
				mOwner->SetTileType(mostBottom->LBIdxX + 2, mostBottom->LBIdxY, eTileType::EntrySERight);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, closeRoom->RTIdxY, eTileType::EntryNWRight);
				mOwner->SetTileType(closeRoom->RTIdxX - 2, closeRoom->RTIdxY, eTileType::EntryNWLeft);

				// �濡�� �ٷ� �̾����� ���
				for (int y = mostBottom->LBIdxY - 1; y >= mLBIdxY; --y)
				{
					mOwner->SetTileType(mostBottom->LBIdxX + 2, y, eTileType::WallNE);
				}
				for (int y = mostBottom->LBIdxY - 1; y > mLBIdxY - 1; --y)
				{
					mOwner->SetTileType(mostBottom->LBIdxX + 1, y, eTileType::WallSW);
				}
				for (int y = closeRoom->RTIdxY + 1; y <= neighbor->mRTIdxY; ++y)
				{
					mOwner->SetTileType(closeRoom->RTIdxX - 2, y, eTileType::WallSW);
				}
				for (int y = closeRoom->RTIdxY + 1; y < neighbor->mRTIdxY + 1; ++y)
				{
					mOwner->SetTileType(closeRoom->RTIdxX - 1, y, eTileType::WallNE);
				}
				
				// ��� ������
				mOwner->SetTileType(mostBottom->LBIdxX + 1, mLBIdxY + 1, eTileType::WallEndSEBottom);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, neighbor->mRTIdxY - 1, eTileType::WallEndNWTop);

				// ��ΰ� �������� ������ �κ�
				mOwner->SetTileType(mostBottom->LBIdxX + 1, mostBottom->LBIdxY - 1, eTileType::WallEndNWBottom);
				mOwner->SetTileType(mostBottom->LBIdxX + 2, mostBottom->LBIdxY - 1, eTileType::WallEndNWTop);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, closeRoom->RTIdxY + 1, eTileType::WallEndSETop);
				mOwner->SetTileType(closeRoom->RTIdxX - 2, closeRoom->RTIdxY + 1, eTileType::WallEndSEBottom);

				// ���� �ڳ�
				mOwner->SetTileType(mostBottom->LBIdxX + 1, mLBIdxY, eTileType::InnerCornerW);
				mOwner->SetTileType(closeRoom->RTIdxX - 1, neighbor->mRTIdxY, eTileType::InnerCornerE);
			}
			// 3. ���� ��η� �̾ �Ǵ� ���
			else
			{
				bool bLeftEntry = rand() > RAND_MAX / 2;
				
				// �� ���� ���� ���ʿ��� ��� ����
				if (bLeftEntry)
				{
					// ��� ����
					for (int y = mostBottom->LBIdxY - 1; y >= closeRoom->RTIdxY + 1; --y)
					{
						mOwner->SetTileType(mostBottom->LBIdxX + 1, y, eTileType::WallSW);
						mOwner->SetTileType(mostBottom->LBIdxX + 2, y, eTileType::WallNE);
					}
					
					// ������ ��ġ
					mOwner->SetTileType(mostBottom->LBIdxX + 1, mostBottom->LBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostBottom->LBIdxX + 2, mostBottom->LBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(mostBottom->LBIdxX + 1, closeRoom->RTIdxY, eTileType::EntryNWLeft);
					mOwner->SetTileType(mostBottom->LBIdxX + 2, closeRoom->RTIdxY, eTileType::EntryNWRight);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostBottom->LBIdxX + 1, mostBottom->LBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(mostBottom->LBIdxX + 2, mostBottom->LBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostBottom->LBIdxX + 1, closeRoom->RTIdxY + 1, eTileType::WallEndSEBottom);
					mOwner->SetTileType(mostBottom->LBIdxX + 2, closeRoom->RTIdxY + 1, eTileType::WallEndSETop);
				}
				// �� ���� ���� �����ʿ��� ��� ����
				else
				{
					// ��� ����
					for (int y = mostBottom->LBIdxY - 1; y >= closeRoom->RTIdxY + 1; --y)
					{
						mOwner->SetTileType(mostBottom->RTIdxX - 1, y, eTileType::WallNE);
						mOwner->SetTileType(mostBottom->RTIdxX - 2, y, eTileType::WallSW);
					}

					// ������ ��ġ
					mOwner->SetTileType(mostBottom->RTIdxX - 1, mostBottom->LBIdxY, eTileType::EntrySERight);
					mOwner->SetTileType(mostBottom->RTIdxX - 2, mostBottom->LBIdxY, eTileType::EntrySELeft);
					mOwner->SetTileType(mostBottom->RTIdxX - 1, closeRoom->RTIdxY, eTileType::EntryNWRight);
					mOwner->SetTileType(mostBottom->RTIdxX - 2, closeRoom->RTIdxY, eTileType::EntryNWLeft);

					// ��ΰ� �������� ������ �κ�
					mOwner->SetTileType(mostBottom->RTIdxX - 1, mostBottom->LBIdxY - 1, eTileType::WallEndNWTop);
					mOwner->SetTileType(mostBottom->RTIdxX - 2, mostBottom->LBIdxY - 1, eTileType::WallEndNWBottom);
					mOwner->SetTileType(mostBottom->RTIdxX - 1, closeRoom->RTIdxY + 1, eTileType::WallEndSETop);
					mOwner->SetTileType(mostBottom->RTIdxX - 2, closeRoom->RTIdxY + 1, eTileType::WallEndSEBottom);
				}
			}
		}
	}
}

RoomInfo* CSpace::GetLeftMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->LBIdxX < room2->LBIdxX)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetRightMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->RTIdxX > room2->RTIdxX)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetTopMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->RTIdxY > room2->RTIdxY)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetBottomMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->LBIdxY < room2->LBIdxY)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetMostCloseRoomInfo(const int centerIdx)
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetMostCloseRoomInfo(centerIdx);
	RoomInfo* room2 = mChild2->GetMostCloseRoomInfo(centerIdx);

	int centerX = centerIdx % mOwner->GetMapCountX();
	int centerY = centerIdx / mOwner->GetMapCountX();
	int x1 = room1->CenterIdx % mOwner->GetMapCountX();
	int y1 = room1->CenterIdx / mOwner->GetMapCountX();
	int x2 = room2->CenterIdx % mOwner->GetMapCountX();
	int y2 = room2->CenterIdx / mOwner->GetMapCountX();

	// �Ÿ� ��	
	int len1 = abs(centerX - x1) + abs(centerY - y1);
	int len2 = abs(centerX - x2) + abs(centerY - y2);

	if (len1 < len2)
	{
		return room1;
	}
	return room2;
}

void CSpace::doPartitioning(const int maxLevel)
{	
	if (mParent)
	{
		mLevel = mParent->mLevel + 1;
	}

	// �������� ���
	mOwner->AddSpace(this, mLevel);

	// �ִ� ���� ������ ������ ��� ���� �ߴ�
	if (maxLevel == mLevel)
	{
		return;
	}

	// �ּ����� ���� ������ ( �ּ� �� ������ + 2 )
	int minSpaceSize = mOwner->GetRoomSizeMin() + 2;

	// �� ������ �ּ� ������ ���� �ΰ��� �� �� ���ٸ�, ������ �ߴ�.
	if (mSizeX < (( mOwner->GetRoomSizeMin() + 2) * 2) || mSizeY < ((mOwner->GetRoomSizeMin() + 2) * 2))
	{
		return;
	}

	mChild1 = new CSpace;
	mChild2 = new CSpace;

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

	// 4 : 6 ~ 6 : 4�� ������ ũ�� �й�
	float ratio = (4.f + (float)(rand() % 3)) / 10.f;

	// 4 : 6 ���� �������� ��, ���� ������ ������ �ּ� ���� ������� ���� ���, 5 : 5�θ� �����Ѵ�.
	if (((mSizeX / 2) * 0.4f <= minSpaceSize) || ((mSizeY / 2 * 0.4f) <= minSpaceSize))
	{
		ratio = 0.5f;
	}
	
	// ����
	if (eSplitDir::Horizontal == dir)
	{
		// child1 �� �Ʒ��� 
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
		// child1�� ����
		mChild1->mLBIdxX = mLBIdxX;
		mChild1->mLBIdxY = mLBIdxY;

		mChild1->mRTIdxX = mLBIdxX + (mSizeX * ratio) - 1;
		mChild1->mRTIdxY = mRTIdxY;

		mChild2->mLBIdxX = mLBIdxX + (mSizeX * ratio);
		mChild2->mLBIdxY = mLBIdxY;

		mChild2->mRTIdxX = mRTIdxX;
		mChild2->mRTIdxY = mRTIdxY;
	}

	// ������ ���
	mChild1->mSizeX = mChild1->mRTIdxX - mChild1->mLBIdxX + 1;
	mChild1->mSizeY = mChild1->mRTIdxY - mChild1->mLBIdxY + 1;

	mChild2->mSizeX = mChild2->mRTIdxX - mChild2->mLBIdxX + 1;
	mChild2->mSizeY = mChild2->mRTIdxY - mChild2->mLBIdxY + 1;

	// ������ �ε��� ���
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

	// �ڽ� ���� ����
	mChild1->doPartitioning(maxLevel);
	mChild2->doPartitioning(maxLevel);
}
