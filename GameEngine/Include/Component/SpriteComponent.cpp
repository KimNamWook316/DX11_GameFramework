#include "SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"

CSpriteComponent::CSpriteComponent()	:
	mAnimation(nullptr)
{
	SetTypeID<CSpriteComponent>();
	mbIsRender = true;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com)
	: CSceneComponent(com)
{
	mMesh = com.mMesh;

	if (com.mAnimation)
	{
		mAnimation = com.mAnimation->Clone();
	}

	if (com.mMaterial)
	{
		mMaterial = com.mMaterial->Clone();
	}
}

CSpriteComponent::~CSpriteComponent()
{
	SAFE_DELETE(mAnimation);
}

void CSpriteComponent::SetMaterial(CMaterial* material)
{
	mMaterial = material->Clone();

	mMaterial->SetScene(mScene);
}

bool CSpriteComponent::Init()
{
	mMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh("SpriteMesh");
	
	// Material이 같은 텍스쳐를 공유하면 안 되기 때문에 SetMaterial로 복사해준다.
	SetMaterial(mScene->GetResource()->FindMaterial("BaseTexture"));

	SetMeshSize(1.f, 1.f, 0.f);
	SetWorldScale((float)mMaterial->GetTextureWidth(), (float)mMaterial->GetTextureHeight(), 1.f);
	return true;
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();

	if (mAnimation)
	{
		mAnimation->Start();
	}
}

void CSpriteComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (mAnimation)
	{
		mAnimation->Update(deltaTime);
	}
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

	// 애니메이션이 있으면
	if (mAnimation)
	{
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(mAnimation->GetAnimationCount() > 0);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();

		mAnimation->SetShader();
	}

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

void CSpriteComponent::Save(FILE* fp)
{
	std::string meshName = mMesh->GetName();
	
	int length = (int)meshName.size();

	fwrite(&length, sizeof(int), 1, fp);
	fwrite(meshName.c_str(), sizeof(char), length, fp);

	mMaterial->Save(fp);

	bool bAnimEnable = false;
	if (mAnimation)
	{
		bAnimEnable = true;
	}

	fwrite(&bAnimEnable, sizeof(bool), 1, fp);

	if (mAnimation)
	{
		mAnimation->Save(fp);
	}

	CSceneComponent::Save(fp);
}

void CSpriteComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
}

void CSpriteComponent::SetBaseColor(const Vector4& color)
{
	mMaterial->SetBaseColor(color);
}

void CSpriteComponent::SetBaseColor(const float r, const float g, const float b, const float a)
{
	mMaterial->SetBaseColor(r, g, b, a);
}

void CSpriteComponent::SetRenderState(CRenderState* state)
{
	mMaterial->SetRenderState(state);
}

void CSpriteComponent::SetRenderState(const std::string& name)
{
	mMaterial->SetRenderState(name);
}

void CSpriteComponent::SetTransparency(bool bEnable)
{
	mMaterial->SetTransparency(bEnable);
}

void CSpriteComponent::SetOpacity(const float val)
{
	mMaterial->SetOpacity(val);
}

void CSpriteComponent::AddOpacity(const float val)
{
	mMaterial->AddOpacity(val);
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

void CSpriteComponent::ChangeAnimation(const std::string& name)
{
	if (mAnimation)
	{
		mAnimation->ChangeAnimation(name);
	}
}

void CSpriteComponent::AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	if (mAnimation)
	{
		mAnimation->AddAnimation(sequenceName, name, bIsLoop, playTime, playScale, bIsReverse);
	}
}

bool CSpriteComponent::CheckCurrentAnimation(const std::string& name)
{
	if (mAnimation)
	{
		return mAnimation->CheckCurrentAnimation(name);
	}
}

void CSpriteComponent::SetPlayTime(const std::string& name, const float playTime)
{
	if (mAnimation)
	{
		mAnimation->SetPlayTime(name, playTime);
	}
}

void CSpriteComponent::SetPlayScale(const std::string& name, const float playScale)
{
	if (mAnimation)
	{
		mAnimation->SetPlayScale(name, playScale);
	}
}

void CSpriteComponent::SetLoop(const std::string& name, bool bLoop)
{
	if (mAnimation)
	{
		mAnimation->SetLoop(name, bLoop);
	}
}

void CSpriteComponent::SetReverse(const std::string& name, bool bReverse)
{
	if (mAnimation)
	{
		mAnimation->SetReverse(name, bReverse);
	}
}

void CSpriteComponent::SetCurrentAnimation(const std::string& name)
{
	if (mAnimation)
	{
		mAnimation->SetCurrentAnimation(name);
	}
}
