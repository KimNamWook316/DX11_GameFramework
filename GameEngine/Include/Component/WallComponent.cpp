#include "WallComponent.h"
#include "Tile.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "CameraComponent.h"
#include "../Resource/Shader/TileMapConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"

CWallComponent::CWallComponent()
{
	SetTypeID<CWallComponent>();
	mbIsRender = true;
	mRenderCount = 0;
	mWallCount = 0;
	mDeltaTime = 0.f;
	mLayerName = "Default";
	mWallInfoBuffer = nullptr;
}

CWallComponent::CWallComponent(const CWallComponent& com)	:
	CSceneComponent(com)
{
	mMesh = com.mMesh;

	if (com.mCBuffer)
	{
		mCBuffer = com.mCBuffer->Clone();
	}

	if (com.mWallInfoBuffer)
	{
		mWallInfoBuffer = com.mWallInfoBuffer->Clone();
	}
}

CWallComponent::~CWallComponent()
{
	SAFE_DELETE(mWallInfoBuffer);
	SAFE_DELETE(mCBuffer);

	size_t size = mVecWall.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecWall[i]);
	}

	mVecWall.clear();
}

bool CWallComponent::Init()
{
	mMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh("SpriteMesh");
	SetMeshSize(1.f, 1.f, 0.f);

	mCBuffer = new CTileMapConstantBuffer;
	mCBuffer->Init();

	return true;
}

void CWallComponent::Start()
{
	CSceneComponent::Start();
}

void CWallComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CWallComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
	mDeltaTime = DeltaTime;
}

void CWallComponent::PrevRender()
{
	CSceneComponent::PrevRender();
	
	if (!mVecWall.empty())
	{
		CCameraComponent* cam = mScene->GetCameraManager()->GetCurrentCamera();

		Matrix matView, matProj;
		matView = cam->GetViewMatrix();
		matProj = cam->GetProjMatrix();

		mRenderCount = 0;
		size_t size = mVecWall.size();

		for (size_t i = 0; i < size; ++i)
		{
			// TileComponent에서 mbRender true처리함
			if (mVecWall[i]->mbRender)
			{
				mVecWallInfo[mRenderCount]->TileStart = mVecWall[i]->GetFrameStart();
				mVecWallInfo[mRenderCount]->TileEnd = mVecWall[i]->GetFrameEnd();
				mVecWallInfo[mRenderCount]->Opacity = mVecWall[i]->GetOpacity();
				mVecWallInfo[mRenderCount]->MatWVP = mVecWall[i]->GetWorldMatrix() * matView * matProj;

				++mRenderCount;

				// 다음 번 렌더를 위해 모두 false 상태로 되돌림
				mVecWall[i]->SetRender(false);
			}
		}

		mWallInfoBuffer->UpdateBuffer(&mVecWallInfo[0], mRenderCount);
	}
}

void CWallComponent::Render()
{
	CSceneComponent::Render();

	if (mTileSet && !mVecWall.empty() && (mRenderCount != 0))
	{
		mWallInfoBuffer->SetShader();
		mCBuffer->UpdateCBuffer();
		mTileSet->Render();
		mMesh->RenderInstancing(mRenderCount);
		mTileSet->Reset();
		mWallInfoBuffer->ResetShader();
	}
}

void CWallComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CWallComponent* CWallComponent::Clone()
{
	return new CWallComponent(*this);
}

void CWallComponent::Save(FILE* fp)
{
}

void CWallComponent::Load(FILE* fp)
{
}

void CWallComponent::CreateWall(CTile* tile)
{
	if (!mTileSet)
	{
		return;
	}

	eTileType type = tile->GetTileType();

	switch (type)
	{
	case eTileType::Normal:
	case eTileType::Wall:
		break;
	case eTileType::CornerN:
	{
 //		// Left, Right 벽
 //		TileSetInfo* info = mTileSet->FindInfo("WallLeft");
 //		CWall* wall = new CWall;
 //		wall->mParentTile = tile;
 //		tile->AddWall(wall);
 //		wall->SetShape(info->Shape);
 //		wall->SetRenderPos(tile->GetPos());
 //		wall->SetSize(tile->GetSize());
 //		mVecWall.push_back(wall);
 //		
 //		info = mTileSet->FindInfo("WallRight");
 //		wall = new CWall;
 //		wall->mParentTile = tile;
 //		tile->AddWall(wall);
 //		wall->SetShape(info->Shape);
 //		wall->SetRenderPos(tile->GetPos());
 //		wall->SetSize(tile->GetSize());
 //		mVecWall.push_back(wall);
 //
		break;
	}
	case eTileType::CornerE:
		break;
	case eTileType::CornerS:
		break;
	case eTileType::CornerW:
		break;
	case eTileType::WallNE:
		break;
	case eTileType::WallSE:
		break;
	case eTileType::WallSW:
		break;
	case eTileType::WallNW:
		break;
	case eTileType::EntryNELeft:
		break;
	case eTileType::EntryNERight:
		break;
	case eTileType::EntryNWLeft:
		break;
	case eTileType::EntryNWRight:
		break;
	default:
		assert(false);
		return;
	}
}

void CWallComponent::ChangeWall(CTile* tile)
{
}

void CWallComponent::SetWorldInfo(const int mTileCount)
{
	SAFE_DELETE(mWallInfoBuffer);

	mWallInfoBuffer = new CStructuredBuffer;
	mWallInfoBuffer->Init("WallInfo", sizeof(TileInfo), mTileCount, 40, true, (int)eBufferShaderTypeFlags::Vertex);
	
	mVecWallInfo.resize(mTileCount);

	for (int i = 0; i < mTileCount; ++i)
	{
		mVecWallInfo[i]->TileColor = Vector4(1.f, 1.f, 1.f, 1.f);
		mVecWallInfo[i]->Opacity = 1.f;
	}
}

void CWallComponent::SetTileSet(CTileSet* tileSet)
{
	mTileSet = tileSet;

	mTileSet->SetScene(mScene);

	if (mTileSet->GetTexture())
	{
		mCBuffer->SetImageSize(Vector2((float)mTileSet->GetTexture()->GetWidth(), (float)mTileSet->GetTexture()->GetHeight()));
	}
}
