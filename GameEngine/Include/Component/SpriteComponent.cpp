#include "SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CSpriteComponent::CSpriteComponent()
{
	SetTypeID<CSpriteComponent>();
	mbIsRender = true;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com)
	: CSceneComponent(com)
{
	mMesh = com.mMesh;
}

CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::SetMaterial(CMaterial* material)
{
	mMaterial = material->Clone();

	mMaterial->SetScene(mScene);
}

bool CSpriteComponent::Init()
{
	mMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh("SpriteMesh");
	
	mMaterial = mScene->GetResource()->FindMaterial("BaseTexture");

	SetMeshSize(1.f, 1.f, 0.f);
	return true;
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();
}

void CSpriteComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CSpriteComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CSpriteComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CSpriteComponent::Render()
{
	// 상수 버퍼 넘기고
	CSceneComponent::Render();

	// 쉐이더 바인드
	mMaterial->Render();

	// 버텍스정보 넘겨서 Draw
	mMesh->Render();

	// 텍스쳐 정보 리셋
	mMaterial->Reset();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::SetBaseColor(const Vector4& color)
{
	mMaterial->SetBaseColor(color);
}

void CSpriteComponent::SetBaseColor(const float r, const float g, const float b, const float a)
{
	mMaterial->SetBaseColor(r, g, b, a);
}

void CSpriteComponent::AddTexture(const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	mMaterial->AddTexture(reg, shaderType, name, texture);
}

void CSpriteComponent::AddTexture(const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	mMaterial->AddTexture(reg, shaderType, name, fileName, pathName);
}

void CSpriteComponent::AddTextureFullPath(const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	mMaterial->AddTextureFullPath(reg, shaderType, name, fullPath);
}

void CSpriteComponent::AddTexture(const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	mMaterial->AddTexture(reg, shaderType, name, vecFileName, pathName);
}

void CSpriteComponent::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	mMaterial->SetTexture(index, reg, shaderType, name, texture);
}

void CSpriteComponent::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{

	mMaterial->SetTexture(index, reg, shaderType, name, fileName, pathName);
}

void CSpriteComponent::SetTextureFullPath(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	mMaterial->SetTextureFullPath(index, reg, shaderType, name, fullPath);
}

void CSpriteComponent::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	mMaterial->SetTexture(index, reg, shaderType, name, vecFileName, pathName);
}
