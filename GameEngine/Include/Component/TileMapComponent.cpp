#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CTileMapComponent::CTileMapComponent()
{
	SetTypeID<CTileMapComponent>();
	mbIsRender = true;
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& comp)	:
	CSceneComponent(comp)
{
	mBackMesh = comp.mBackMesh;
	
	if (comp.mBackMaterial)
	{
		mBackMaterial = comp.mBackMaterial->Clone();
	}
}

CTileMapComponent::~CTileMapComponent()
{
}

bool CTileMapComponent::Init()
{
	mBackMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh("SpriteMesh");
	SetMeshSize(1.f, 1.f, 0.f);

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
}

void CTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileMapComponent* CTileMapComponent::Clone()
{
	return new CTileMapComponent(*this);
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

void CTileMapComponent::SetMaterial(CMaterial* material)
{
	mBackMaterial = material->Clone();
	mBackMaterial->SetScene(mScene);
}
