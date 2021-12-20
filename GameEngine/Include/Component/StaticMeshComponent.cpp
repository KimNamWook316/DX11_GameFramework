#include "StaticMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	SetTypeID<CStaticMeshComponent>();
	mbIsRender = true;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com)
	: CSceneComponent(com)
{
	mMesh = com.mMesh;

	if (com.mMaterial)
	{
		mMaterial = com.mMaterial->Clone();
	}
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::SetMesh(const std::string& name)
{
	mMesh = (CStaticMesh*)mScene->GetResource()->FindMesh(name);
}

void CStaticMeshComponent::SetMesh(CStaticMesh* mesh)
{
	mMesh = mesh;
}

void CStaticMeshComponent::SetMaterial(CMaterial* material)
{
	mMaterial = material->Clone();

	mMaterial->SetScene(mScene);
}

bool CStaticMeshComponent::Init()
{
	//mMesh = (CStaticMesh*)mScene->GetResource()->FindMesh("FrameRectMesh");
	
	// Material이 같은 텍스쳐를 공유하면 안 되기 때문에 SetMaterial로 복사해준다.
	SetMaterial(mScene->GetResource()->FindMaterial("Color"));

	return true;
}

void CStaticMeshComponent::Start()
{
	CSceneComponent::Start();
}

void CStaticMeshComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CStaticMeshComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CStaticMeshComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CStaticMeshComponent::Render()
{
	// 상수 버퍼 넘기고
	CSceneComponent::Render();

	if (!mMesh || !mMaterial)
	{
		return;
	}

	// 쉐이더 바인드
	mMaterial->Render();

	// 버텍스정보 넘겨서 Draw
	mMesh->Render();

	// 텍스쳐 정보 리셋
	mMaterial->Reset();
}

void CStaticMeshComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* fp)
{
	std::string meshName = mMesh->GetName();
	int length = (size_t)meshName.length();

	fwrite(&length, sizeof(int), 1, fp);
	fwrite(meshName.c_str(), sizeof(char), length, fp);

	mMaterial->Save(fp);
	CSceneComponent::Save(fp);
}

void CStaticMeshComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
}

void CStaticMeshComponent::SetBaseColor(const Vector4& color)
{
	mMaterial->SetBaseColor(color);
}

void CStaticMeshComponent::SetBaseColor(const float r, const float g, const float b, const float a)
{
	mMaterial->SetBaseColor(r, g, b, a);
}

void CStaticMeshComponent::SetRenderState(CRenderState* state)
{
	mMaterial->SetRenderState(state);
}

void CStaticMeshComponent::SetRenderState(const std::string& name)
{
	mMaterial->SetRenderState(name);
}

void CStaticMeshComponent::SetTransparency(bool bEnable)
{
	mMaterial->SetTransparency(bEnable);
}

void CStaticMeshComponent::SetOpacity(const float val)
{
	mMaterial->SetOpacity(val);
}

void CStaticMeshComponent::AddOpacity(const float val)
{
	mMaterial->AddOpacity(val);
}

void CStaticMeshComponent::AddTexture(const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	mMaterial->AddTexture(reg, shaderType, name, texture);
}

void CStaticMeshComponent::AddTexture(const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	mMaterial->AddTexture(reg, shaderType, name, fileName, pathName);
}

void CStaticMeshComponent::AddTextureFullPath(const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	mMaterial->AddTextureFullPath(reg, shaderType, name, fullPath);
}

void CStaticMeshComponent::AddTexture(const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	mMaterial->AddTexture(reg, shaderType, name, vecFileName, pathName);
}

void CStaticMeshComponent::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	mMaterial->SetTexture(index, reg, shaderType, name, texture);
}

void CStaticMeshComponent::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{

	mMaterial->SetTexture(index, reg, shaderType, name, fileName, pathName);
}

void CStaticMeshComponent::SetTextureFullPath(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
	mMaterial->SetTextureFullPath(index, reg, shaderType, name, fullPath);
}

void CStaticMeshComponent::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	mMaterial->SetTexture(index, reg, shaderType, name, vecFileName, pathName);
}
