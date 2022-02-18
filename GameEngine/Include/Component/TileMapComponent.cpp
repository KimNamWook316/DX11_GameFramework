#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Shader/TileMapConstantBuffer.h"
#include "../Scene/NavigationManager.h"
#include "CameraComponent.h"

CTileMapComponent::CTileMapComponent()
{
	SetTypeID<CTileMapComponent>();
	mbIsRender = true;

	mCountX = 0;
	mCountY = 0;
	mRenderCount = 0;
	mTileDiagonal = 0.f;
	meTileShape = eTileShape::Rect;
	mLayerName = "Back";
	mTileInfoBuffer = nullptr;

	for (int i = 0; i < (int)eTileType::Max; ++i)
	{
		mTileColor[i] = Vector4(1.f, 1.f, 1.f, 1.f);
	}

	mTileColor[(int)eTileType::Wall] = Vector4(1.f, 0.f, 0.f, 1.f);
	mbEditMode = false;
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

	if (comp.mTileInfoBuffer)
	{
		mTileInfoBuffer = comp.mTileInfoBuffer->Clone();
	}

	for (int i = 0; i < (int)eTileType::Max; ++i)
	{
		mTileColor[i] = comp.mTileColor[i];
	}
	
	mbEditMode = comp.mbEditMode;
}

CTileMapComponent::~CTileMapComponent()
{
	SAFE_DELETE(mTileInfoBuffer);
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

	if (!mVecTile.empty())
	{
		CCameraComponent* camera = mScene->GetCameraManager()->GetCurrentCamera();

		Resolution RS = camera->GetResolution();

		Vector3 LB = camera->GetWorldPos();
		Vector3 RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);
		Vector3 LT = Vector3(LB.x, RT.y, 0.f);
		Vector3 RB = Vector3(RT.x, LB.y, 0.f);

		int startX, startY, endX, endY;

		startX = getTileRenderIndexX(LB);
		endX = getTileRenderIndexX(RT);

		if (eTileShape::Rect == meTileShape)
		{
			startY = getTileRenderIndexY(LB);
			endY = getTileRenderIndexY(RT);
		}
		else if (eTileShape::Rhombus == meTileShape)
		{
			startY = getTileRenderIndexY(RB);
			endY = getTileRenderIndexY(LT);
		}

		Matrix matView, matProj;
		matView = camera->GetViewMatrix();
		matProj = camera->GetProjMatrix();

		mRenderCount = 0;

		for (int i = startY; i <= endY; ++i)
		{
			for (int j = startX; j <= endX; ++j)
			{
				int index = i * mCountX + j;
				mVecTile[index]->Update(DeltaTime);

				if (mVecTile[index]->GetIsRender())
				{
					if (mbEditMode)
					{
						mVecTileInfo[mRenderCount].TileColor = mTileColor[(int)mVecTile[index]->GetTileType()];
					}

					mVecTileInfo[mRenderCount].TileStart = mVecTile[index]->GetFrameStart();
					mVecTileInfo[mRenderCount].TileEnd = mVecTile[index]->GetFrameEnd();
					mVecTileInfo[mRenderCount].Opacity = mVecTile[index]->GetOpacity();
					mVecTileInfo[mRenderCount].MatWVP = mVecTile[index]->GetWorldMatrix() * matView * matProj;
					mVecTileInfo[mRenderCount].MatWVP.Transpose();
					++mRenderCount;
				}
			}
		}

		mTileInfoBuffer->UpdateBuffer(&mVecTileInfo[0], mRenderCount);
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

	if (mTileMaterial && !mVecTile.empty())
	{
		mTileInfoBuffer->SetShader();
		mCBuffer->UpdateCBuffer();
		mTileMaterial->Render();
		mBackMesh->RenderInstancing(mRenderCount);
		mTileMaterial->Reset();
		mTileInfoBuffer->ResetShader();
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

int CTileMapComponent::GetTileIndexX(const Vector3& pos)
{
	if (eTileShape::Rect == meTileShape)
	{
		float convertX = pos.x - GetWorldPos().x;

		int idxX = (int)(convertX / mTileSize.x);

		if (idxX < 0 || idxX >= mCountX)
		{
			return -1;
		}
		else
		{
			return idxX;
		}
	}
	else if (eTileShape::Rhombus == meTileShape)
	{
		Vector3 convertPos = Vector3(pos - GetWorldPos()).TransformCoord(mMatWorldToIso);
		
		int idxX = (int)(convertPos.x / mTileDiagonal);

		if (idxX < 0 || idxX >= mCountX)
		{
			return -1;
		}
		else
		{
			return idxX;
		}
	}
	return -1;
}

int CTileMapComponent::GetTileIndexY(const Vector3& pos)
{
	if (eTileShape::Rect == meTileShape)
	{
		float convertY = pos.y - GetWorldPos().y;

		int idxY = (int)(convertY / mTileSize.y);

		if (idxY < 0 || idxY >= mCountY)
		{
			return -1;
		}
		else
		{
			return idxY;
		}
	}
	else if (eTileShape::Rhombus == meTileShape)
	{
		Vector3 convertPos = Vector3(pos - GetWorldPos()).TransformCoord(mMatWorldToIso);
		
		int idxY = (int)(convertPos.y / mTileDiagonal);

		if (idxY < 0 || idxY >= mCountY)
		{
			return -1;
		}
		else
		{
			return idxY;
		}
	}
	return -1;
}

int CTileMapComponent::GetTileIndex(const Vector3& pos)
{
	int idxX = GetTileIndexX(pos);
	int idxY = GetTileIndexY(pos);

	if (-1 == idxX || -1 == idxY)
	{
		return -1;
	}

	return idxY * mCountX + idxX;
}

CTile* CTileMapComponent::GetTile(const Vector3& pos)
{
	int idx = GetTileIndex(pos);

	if (-1 == idx)
	{
		return nullptr;
	}
	
	return mVecTile[idx];
}

CTile* CTileMapComponent::GetTile(const int x, const int y)
{
	if (x < 0 || x >= mCountX || y < 0 || y >= mCountY)
	{
		return nullptr;
	}

	return mVecTile[y * mCountY + x];
}

CTile* CTileMapComponent::GetTile(const int idx)
{
	if (idx < 0 || idx > mCount)
	{
		return nullptr;
	}

	return mVecTile[idx];
}

void CTileMapComponent::CreateTile(eTileShape eShape, const int countX, const int countY, const Vector2& size)
{
	ClearTile();

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
		mTileDiagonal = sqrt((mTileSize.x / 2.f) * (mTileSize.x / 2.f) +
			(mTileSize.y / 2.f) * (mTileSize.y / 2.f));

		mMatIsoToWorld.v[0] = Vector4((2.f * sqrt(5.f)/ 5.f) - 0.01f , (sqrt(5.f) / 5.f) - 0.01f, 0.f, 0.f);
		mMatIsoToWorld.v[1] = Vector4((-2.f * sqrt(5.f) / 5.f) + 0.01f, (sqrt(5.f) / 5.f) - 0.01f, 0.f, 0.f);
		mMatIsoToWorld.v[2] = Vector4(0.f, 0.f, 1.f, 0.f);
		mMatIsoToWorld.v[3] = Vector4(0.f, 0.f, 0.f, 1.f);

		mMatWorldToIso = mMatIsoToWorld;
		mMatWorldToIso.Inverse();

		Vector3 pos;

		for (int i = 0; i < mCountY; ++i)
		{
			for (int j = 0; j < mCountX; ++j)
			{
				pos = Vector3((float)j * mTileDiagonal, (float)i * mTileDiagonal, 0).TransformCoord(mMatIsoToWorld);
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
	mCount = mCountX * mCountY;
	SetWorldInfo();

	if (mTileMaterial)
	{
		Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
		Vector2 imageStart(0.f, 0.f);
		mCBuffer->SetImageSize(imageSize);
		mCBuffer->SetImageStart(Vector2(0.f, 0.f));
		mCBuffer->SetImageEnd(imageSize);
		SetTileDefaultFrame(imageStart, imageSize);
	}

	mScene->GetNavigationManager()->SetNavData(this);
}

void CTileMapComponent::ClearTile()
{
	size_t size = mVecTile.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecTile[i]);
	}
	mVecTile.clear();
}

void CTileMapComponent::SetWorldInfo()
{
	SAFE_DELETE(mTileInfoBuffer);

	mTileInfoBuffer = new CStructuredBuffer;

	// CPU에서 일괄적으로 쓰고 보낼것이기 때문에 dynamic
	mTileInfoBuffer->Init("TileInfo", sizeof(TileInfo), mCountX * mCountY, 40, true, (int)eBufferShaderTypeFlags::Vertex);

	mVecTileInfo.resize(mCount);

	for (int i = 0; i < mCount; ++i)
	{
		mVecTileInfo[i].TileColor = Vector4(1.f, 1.f, 1.f, 1.f);
		mVecTileInfo[i].Opacity = 1.f;
	}
}

void CTileMapComponent::SetTileDefaultFrame(const Vector2& start, const Vector2& end)
{
	for (int i = 0; i < mCount; ++i)
	{
		mVecTile[i]->SetFrameStart(start);
		mVecTile[i]->SetFrameEnd(end);
	}
}

void CTileMapComponent::SetTileDefaultFrame(const float startX, const float startY, const float endX, const float endY)
{
	for (int i = 0; i < mCount; ++i)
	{
		mVecTile[i]->SetFrameStart(Vector2(startX, startY));
		mVecTile[i]->SetFrameEnd(Vector2(endX, endY));
	}
}

void CTileMapComponent::SetTileFrame(const int idxX, const int idxY, const float startX, const float startY, const float endX, const float endY)
{
	mVecTile[idxY * mCountX + idxX]->SetFrameStart(Vector2(startX, startY));
	mVecTile[idxY * mCountX + idxX]->SetFrameEnd(Vector2(endX, endY));
}

void CTileMapComponent::SetTileFrame(const int idx, const float startX, const float startY, const float endX, const float endY)
{
	mVecTile[idx]->SetFrameStart(Vector2(startX, startY));
	mVecTile[idx]->SetFrameEnd(Vector2(endX, endY));
}

void CTileMapComponent::SetTileFrame(const Vector3& pos, const float startX, const float startY, const float endX, const float endY)
{
}

void CTileMapComponent::SetTileOpacity(const int idxX, const int idxY, const float opacity)
{
	mVecTile[idxY * mCountX + idxX]->SetOpacity(opacity);
}

void CTileMapComponent::SetTileOpacity(const int idx, const float opacity)
{
	mVecTile[idx]->SetOpacity(opacity);
}

void CTileMapComponent::SetTileOpacity(const Vector3& pos, const float opacity)
{
}

void CTileMapComponent::SetTileColor(eTileType type, const float r, const float g, const float b, const float a)
{
	mTileColor[(int)type] = Vector4(r, g, b, a);
}

void CTileMapComponent::SetTileColor(eTileType type, const Vector4& color)
{
	mTileColor[(int)type] = color;
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

void CTileMapComponent::AddTileTexture(const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	if (!mTileMaterial)
	{
		return;
	}
	mTileMaterial->AddTexture(reg, shaderType, name, texture);
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageStart);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::AddTileTexture(const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!mTileMaterial)
	{
		return;
	}
	mTileMaterial->AddTexture(reg, shaderType, name, fileName, pathName);
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageStart);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::AddTileTextureFullPath(const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!mTileMaterial)
	{
		return;
	}
	mTileMaterial->AddTextureFullPath(reg, shaderType, name, fullPath);
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageStart);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::AddTileTexture(const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (!mTileMaterial)
	{
		return;
	}
	mTileMaterial->AddTexture(reg, shaderType, name, vecFileName);
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageStart);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::SetTileTexture(const int index, const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	if (!mTileMaterial)
	{
		return;
	}
	mTileMaterial->SetTexture(index, reg, shaderType, name, texture);
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageStart);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::SetTileTexture(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!mTileMaterial)
	{
		return;
	}
	mTileMaterial->SetTexture(index, reg, shaderType, name, fileName, pathName);
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageSize);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::SetTileTextureFullPath(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!mTileMaterial)
	{
		return;
	}
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageSize);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
}

void CTileMapComponent::SetTileTexture(const int index, const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (!mTileMaterial)
	{
		return;
	}
	Vector2 imageSize((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight());
	Vector2 imageStart(0.f, 0.f);
	mCBuffer->SetImageSize(imageSize);
	mCBuffer->SetImageStart(Vector2(0.f, 0.f));
	mCBuffer->SetImageEnd(imageSize);
	SetTileDefaultFrame(imageStart, imageSize);
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
	
	if (!mTileMaterial->IsTextureEmpty())
	{
		mCBuffer->SetImageSize(Vector2((float)mTileMaterial->GetTextureWidth(), (float)mTileMaterial->GetTextureHeight()));
	}
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
		Vector3 convertPos = pos - GetWorldPos();
		convertPos = convertPos.TransformCoord(mMatWorldToIso);

		int idxX = (int)convertPos.x / mTileDiagonal;

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
		else if (idxY >= mCountY)
		{
			return mCountY - 1;
		}

		return idxY;
	}
	else if (eTileShape::Rhombus == meTileShape)
	{
		Vector3 convertPos = pos - GetWorldPos();
		convertPos = convertPos.TransformCoord(mMatWorldToIso);

		int idxY = (int)convertPos.y / mTileDiagonal;

		if (idxY < 0)
		{
			return 0;
		}
		else if (idxY >= mCountY)
		{
			return mCountY - 1;
		}
		return idxY;
	}
	return 0;
}
