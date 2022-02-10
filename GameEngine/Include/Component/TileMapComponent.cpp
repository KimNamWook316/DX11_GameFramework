#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "CameraComponent.h"
#include "../Resource/Shader/TileMapConstantBuffer.h"

CTileMapComponent::CTileMapComponent()
{
	SetTypeID<CTileMapComponent>();
	mbIsRender = true;

	mCountX = 0;
	mCountY = 0;
	mRenderCount = 0;
	meTileShape = eTileShape::Rect;
	mLayerName = "Back";
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& comp)	:
	CSceneComponent(comp)
{
	mBackMesh = comp.mBackMesh;
	
	if (comp.mBackMaterial)
	{
		mBackMaterial = comp.mBackMaterial->Clone();
	}

	if (comp.mTileMaterial)
	{
		mTileMaterial = comp.mTileMaterial->Clone();
	}

	if (comp.mCBuffer)
	{
		mCBuffer = comp.mCBuffer->Clone();
	}
}

CTileMapComponent::~CTileMapComponent()
{
	SAFE_DELETE(mCBuffer);

	size_t size = mVecTile.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecTile[i]);
	}

	mVecTile.clear();
}

bool CTileMapComponent::Init()
{
	mBackMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh("SpriteMesh");
	SetMeshSize(1.f, 1.f, 0.f);

	mCBuffer = new CTileMapConstantBuffer;
	mCBuffer->Init();

	return true;
}

void CTileMapComponent::Start()
{
	CSceneComponent::Start();
}

void CTileMapComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	CCameraComponent* camera = mScene->GetCameraManager()->GetCurrentCamera();

	Resolution RS = camera->GetResolution();

	Vector3 LB = camera->GetWorldPos();
	Vector3 RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

	int startX, startY, endX, endY;

	startX = getTileRenderIndexX(LB);
	startY = getTileRenderIndexY(LB);
	endX = getTileRenderIndexX(RT);
	endY = getTileRenderIndexY(RT);

	mRenderCount = (endX - startX + 1) * (endY - startY + 1);

	for (int i = startY; i <= endY; ++i)
	{
		for (int j = startX; j <= endX; ++j)
		{
			int index = i * mCountX + j;
			mVecTile[index]->Update(DeltaTime);
		}
	}
}

void CTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CTileMapComponent::Render()
{
	CSceneComponent::Render();

	if (mBackMaterial)
	{
		mBackMaterial->Render();
		mBackMesh->Render();
		mBackMaterial->Reset();
	}

	if (mTileMaterial)
	{
		CCameraComponent* camera = mScene->GetCameraManager()->GetCurrentCamera();

		Resolution RS = camera->GetResolution();

		Vector3 LB = camera->GetWorldPos();
		Vector3 RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

		int startX, endX, startY, endY;

		startX = getTileRenderIndexX(LB);
		startY = getTileRenderIndexY(LB);
		endX = getTileRenderIndexX(RT);
		endY = getTileRenderIndexY(RT);

		mRenderCount = (endX - startX + 1) * (endY - startY + 1);

		for (int i = startY; i <= endY; ++i)
		{
			for (int j = startX; j <= endX; ++j)
			{
				int idx = i * mCountX + j;

				mCBuffer->SetImageStart(Vector2(0.f, 0.f));
				mCBuffer->SetImageEnd(Vector2(64.f, 64.f));

				Matrix matWVP = mVecTile[idx]->GetWorldMatrix();

				matWVP *= camera->GetViewMatrix();
				matWVP *= camera->GetProjMatrix();

				matWVP.Transpose();

				mCBuffer->SetWVP(matWVP);

				mTileMaterial->Render();
				mBackMesh->Render(); // ??
				mTileMaterial->Reset();
			}
		}
	}
}

void CTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileMapComponent* CTileMapComponent::Clone()
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::CreateTile(eTileShape eShape, const int countX, const int countY, const Vector2& size)
{
	meTileShape = eShape;
	mCountX = countX;
	mCountY = countY;
	mTileSize = size;

	mVecTile.resize((size_t)mCountX * (size_t)mCountY);

	for (int i = 0; i < mCountY; ++i)
	{
		for (int j = 0; j < mCountX; ++j)
		{
			CTile* tile = new CTile;

			tile->mOwner = this;
			
			int idx = i * mCountX + j;

			tile->SetIndex(j, i, idx);
			tile->SetSize(mTileSize);
			tile->SetShape(meTileShape);

			mVecTile[idx] = tile;
		}
	}

	switch (meTileShape)
	{
	case eTileShape::Rect:
	{
		Vector3 pos;

		for (int i = 0; i < mCountY; ++i)
		{
			pos.x = 0.f;
			pos.y = i * mTileSize.y;

			for (int j = 0; j < mCountX; ++j)
			{
				pos.x = j * mTileSize.x;
				int idx = i * mCountX + j;
				mVecTile[idx]->SetPos(pos);
			}
		}
	}
		break;
	case eTileShape::Rhombus:
	{
		Vector3 pos;
		Matrix matIsometric;
		matIsometric.v[0] = Vector4(sqrt(3.f) / 2.f, -sqrt(3.f) / 2.f, 0.f, 0.f);
		matIsometric.v[1] = Vector4(0.5f, 0.5f, 0.f, 0.f);
		matIsometric.v[2] = Vector4(0.f, 0.f, 0.f, 0.f);
		matIsometric.v[3] = Vector4(0.f, 0.f, 0.f, 1.f);

		for (int i = 0; i < mCountY; ++i)
		{
			for (int j = 0; j < mCountX; ++j)
			{
				pos = Vector3(j, i, 0).TransformCoord(matIsometric);
				int idx = i * mCountX + j;
				mVecTile[idx]->SetPos(pos);
			}
		}
	}
		break;
	default:
		assert(false);
		break;
	}

	mCBuffer->SetTileSize(Vector2(mTileSize.x, mTileSize.y));
}

void CTileMapComponent::SetBackBaseColor(const Vector4& color)
{
	if (!mBackMaterial)
	{
		return;
	}

	mBackMaterial->SetBaseColor(color);
}

void CTileMapComponent::SetBackBaseColor(const float r, const float g, const float b, const float a)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetBaseColor(r,g,b,a);
}

void CTileMapComponent::SetBackRenderState(CRenderState* state)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetRenderState(state);
}

void CTileMapComponent::SetBackRenderState(const std::string& name)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetRenderState(name);
}

void CTileMapComponent::SetBackTransparency(bool bEnable)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetTransparency(bEnable);
}

void CTileMapComponent::SetBackOpacity(const float val)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetOpacity(val);
}

void CTileMapComponent::AddBackOpacity(const float val)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->AddOpacity(val);
}

void CTileMapComponent::AddBackTexture(const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->AddTexture(reg, shaderType, name, texture);
}

void CTileMapComponent::AddBackTexture(const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->AddTexture(reg, shaderType, name, fileName, pathName);
}

void CTileMapComponent::AddBackTextureFullPath(const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->AddTextureFullPath(reg, shaderType, name, fullPath);
}

void CTileMapComponent::AddBackTexture(const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->AddTexture(reg, shaderType, name, vecFileName, pathName);
}

void CTileMapComponent::SetBackTexture(const int index, const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetTexture(index, reg, shaderType, name, texture);
}

void CTileMapComponent::SetBackTexture(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetTexture(index, reg, shaderType, name, fileName, pathName);
}

void CTileMapComponent::SetBackTextureFullPath(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetTextureFullPath(index, reg, shaderType, name, fullPath);
}

void CTileMapComponent::SetBackTexture(const int index, const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (!mBackMaterial)
	{
		return;
	}
	mBackMaterial->SetTexture(index, reg, shaderType, name, vecFileName, pathName);
}

void CTileMapComponent::Save(FILE* fp)
{
	std::string meshName = mBackMesh->GetName();

	int length = (int)meshName.length();

	fwrite(&length, sizeof(int), 1, fp);
	fwrite(meshName.c_str(), sizeof(char), length, fp);

	mBackMesh->Save(fp);
	
	CSceneComponent::Save(fp);
}

void CTileMapComponent::Load(FILE* fp)
{
	char meshName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, fp);
	fread(meshName, sizeof(char), length, fp);

	mBackMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh(meshName);
	mBackMaterial = mScene->GetResource()->CreateMaterialEmpty<CMaterial>();
	mBackMaterial->Load(fp);

	CSceneComponent::Load(fp);
}

void CTileMapComponent::SetBackMaterial(CMaterial* material)
{
	mBackMaterial = material->Clone();
	mBackMaterial->SetScene(mScene);
}

void CTileMapComponent::SetTileMaterial(CMaterial* material)
{
	mTileMaterial = material->Clone();

	mTileMaterial->SetScene(mScene);
	
	mCBuffer->SetImageSize(Vector2((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight()));
}

int CTileMapComponent::getTileRenderIndexX(const Vector3& pos)
{
	if (eTileShape::Rect == meTileShape)
	{
		float convertX = pos.x - GetWorldPos().x;

		int idxX = (int)(convertX / mTileSize.x);

		if (idxX < 0)
		{
			return 0;
		}
		else if (idxX >= mCountX)
		{
			return mCountX - 1;
		}

		return idxX;
	}
	else if (eTileShape::Rhombus == meTileShape)
	{

	}
	else
	{
		return 0;
	}
	return 0;
}

int CTileMapComponent::getTileRenderIndexY(const Vector3& pos)
{
	if (eTileShape::Rect == meTileShape)
	{
		float convertY = pos.y - GetWorldPos().y;

		int idxY = (int)(convertY / mTileSize.y);

		if (idxY < 0)
		{
			return 0;
		}
		else if (idxY >= mCountX)
		{
			return mCountY - 1;
		}

		return idxY;
	}
	else if (eTileShape::Rhombus == meTileShape)
	{

	}
	else
	{
		return 0;
	}
	return 0;
}
