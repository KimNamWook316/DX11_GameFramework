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
				mVecWall[i]->Update();
				mVecWallInfo[mRenderCount].TileStart = mVecWall[i]->GetFrameStart();
				mVecWallInfo[mRenderCount].TileEnd = mVecWall[i]->GetFrameEnd();
				mVecWallInfo[mRenderCount].Opacity = mVecWall[i]->GetOpacity();
				mVecWallInfo[mRenderCount].MatWVP = mVecWall[i]->GetWorldMatrix() * matView * matProj;
				mVecWallInfo[mRenderCount].MatWVP.Transpose();

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

bool CWallComponent::createWallFromTile(CTile* tile)
{
	eTileType type = tile->GetTileType();

	CTileMapComponent* tilemap = static_cast<CTileMapComponent*>(mParent);
	TileSetInfo* info = nullptr;
	CWall* wall = nullptr;
	Vector3 pos;
	Vector2 size;

	switch (type)
	{
	case eTileType::Normal:
	case eTileType::Wall:
		break;
	case eTileType::CornerN:
		info = mTileSet->FindInfo("CornerNLeft");
		wall = new CWall;
		size = info->ImageEnd - info->ImageStart;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerNRight");
		wall = new CWall;
		size = info->ImageEnd - info->ImageStart;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::CornerE:
		info = mTileSet->FindInfo("Wall11");
		wall = new CWall;
		size = info->ImageEnd - info->ImageStart;
		wall->SetInfo(this ,tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerNE");
		wall = new CWall;
		size = info->ImageEnd - info->ImageStart;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this,tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::CornerS:
		info = mTileSet->FindInfo("Wall2");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("Wall11");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("Pillar");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::CornerW:
		info = mTileSet->FindInfo("Wall2");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerNW");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f,  0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::InnerCornerN:
	case eTileType::InnerCornerE:
	case eTileType::InnerCornerS:
	case eTileType::InnerCornerW:
		break;
	case eTileType::WallNE:
		// 기둥이 향하는 방향 : 11시방향
		info = mTileSet->FindInfo("Wall11");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallSE:
		info = mTileSet->FindInfo("Wall2");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallSW:
		info = mTileSet->FindInfo("Wall11");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallNW:
		info = mTileSet->FindInfo("Wall2");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndNETop:
		info = mTileSet->FindInfo("CornerNE");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadRight");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndNEBottom:
		info = mTileSet->FindInfo("CornerNE");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadRight");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndSETop:
		info = mTileSet->FindInfo("Wall11");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("Pillar");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadLeft");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndSEBottom:
		info = mTileSet->FindInfo("Wall11");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("Pillar");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadLeft");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndSWTop:
		info = mTileSet->FindInfo("Wall2");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("Pillar");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(-1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadRight");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(-1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndSWBottom:
		info = mTileSet->FindInfo("Wall2");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("Pillar");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(-1.f);
		mVecWall.push_back(wall);
		
		info = mTileSet->FindInfo("CornerHeadRight");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), -tilemap->GetTileDiagonal(), 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(-1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndNWTop:
		info = mTileSet->FindInfo("CornerNW");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(-1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadLeft");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(-1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::WallEndNWBottom:
		info = mTileSet->FindInfo("CornerNW");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(),0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->SubSortY(-1.f);
		mVecWall.push_back(wall);

		info = mTileSet->FindInfo("CornerHeadLeft");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(),0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		wall->AddSortY(-1.f);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntryNELeft:
		info = mTileSet->FindInfo("Entry11Left");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntryNERight:
		info = mTileSet->FindInfo("Entry11Right");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntrySELeft:
		info = mTileSet->FindInfo("Entry2Left");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(),  0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntrySERight:
		info = mTileSet->FindInfo("Entry2Right");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(0.f, -tilemap->GetTileDiagonal(),  0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this,tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntrySWLeft:
		info = mTileSet->FindInfo("Entry11Left");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntrySWRight:
		info = mTileSet->FindInfo("Entry11Right");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		pos = tile->GetPos() + Vector3(-tilemap->GetTileDiagonal(), 0.f, 0.f).TransformCoord(tilemap->GetMatIsoToWorld());
		wall->SetInfo(this, tile, info->Shape, pos, size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntryNWLeft:
		info = mTileSet->FindInfo("Entry2Left");
		wall = new CWall;
		size = info->ImageEnd - info->ImageStart;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	case eTileType::EntryNWRight:
		info = mTileSet->FindInfo("Entry2Right");
		size = info->ImageEnd - info->ImageStart;
		wall = new CWall;
		wall->SetInfo(this, tile, info->Shape, tile->GetPos(), size);
		wall->SetFrameStart(info->ImageStart);
		wall->SetFrameEnd(info->ImageEnd);
		mVecWall.push_back(wall);
		break;
	default:
		assert(false);
		return false;
	}
	return true;
}

bool CWallComponent::CreateWall(const std::vector<class CTile*>& vecTile)
{
	if (!mTileSet)
	{
		return false;
	}

	size_t size = vecTile.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (!createWallFromTile(vecTile[i]))
		{
			return false;
		}
	}

	SetWorldInfo((int)size);
	return true;
}

void CWallComponent::SetWorldInfo(const int tileCount)
{
	SAFE_DELETE(mWallInfoBuffer);

	mWallInfoBuffer = new CStructuredBuffer;
	mWallInfoBuffer->Init("WallInfo", sizeof(TileInfo), tileCount, 40, true, (int)eBufferShaderTypeFlags::Vertex);
	
	mVecWallInfo.resize(tileCount);

	for (int i = 0; i < tileCount; ++i)
	{
		mVecWallInfo[i].TileColor = Vector4(1.f, 1.f, 1.f, 1.f);
		mVecWallInfo[i].Opacity = 1.f;
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
