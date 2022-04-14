#include "D2ShadowComponent.h"
#include "Component/SpriteComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Render/RenderManager.h"
#include "Resource/Shader/Standard2DConstantBuffer.h"

CD2ShadowComponent::CD2ShadowComponent()	:
	mSprite(nullptr),
	mAnimation(nullptr),
	mOffset(0.f, 0.f)
{
	SetTypeID<CD2ShadowComponent>();

	mbIsRender = true;
}

CD2ShadowComponent::CD2ShadowComponent(const CD2ShadowComponent& com) :
	CSceneComponent(com)
{
	mMesh = com.mMesh;
	mOffset = com.mOffset;
}

CD2ShadowComponent::~CD2ShadowComponent()
{
}

bool CD2ShadowComponent::Init()
{
	CSceneComponent::Init();

	mMesh = (CMesh*)mScene->GetResource()->FindMesh("ShadowMesh");
	SetMeshSize(1.f, 1.f, 0.f);
	mShader = (CGraphicShader*)mScene->GetResource()->FindShader("ShadowShader");

	mTransform->SetBoolInheritPosX(true);
	mTransform->SetBoolInheritPosY(true);
	mTransform->SetBoolInheritRotX(true);
	mTransform->SetBoolInheritRotY(true);
	mTransform->SetBoolInheritRotZ(true);
	mTransform->SetBoolInheritScale(true);

	mTransform->SetWorldScale(1.f, 1.f, 1.f);

	return true;
}

void CD2ShadowComponent::Start()
{
	CSceneComponent::Start();

	if (!mMesh)
	{
		mMesh = (CMesh*)mScene->GetResource()->FindMesh("ShadowMesh");
		SetMeshSize(1.f, 1.f, 0.f);
	}

	if (!mShader)
	{
		mShader = (CGraphicShader*)mScene->GetResource()->FindShader("ShadowShader");
	}

	mTransform->FixZ(true);
	mTransform->SetFixZPos(999.f);

	mSprite = (CSpriteComponent*)(mObject->FindSceneComponentFromType<CSpriteComponent>());
	mAnimation = mSprite->GetAnimationInstance();
	mTransform->SetPivot(mSprite->GetPivot());
}

void CD2ShadowComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CD2ShadowComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CD2ShadowComponent::Render()
{
	if (!mEnable)
	{
		return;
	}

	Vector3 worldPos = mTransform->GetWorldPos();

	mTransform->SetRelativePos(mOffset.x, mOffset.y, 0.f);

	// 타일보다 위, 이외 모든 오브젝트보다 아래에 깔린다.
	CSceneComponent::Render();

	mTexture = mSprite->GetAnimationInstance()->GetCurrentAnimationTexture();

	mShader->SetShader();

	mTexture->SetShader(0, (int)eBufferShaderTypeFlags::Pixel, 0);

	if (mAnimation)
	{
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(mAnimation->GetAnimationCount() > 0);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
		mAnimation->SetShader();
	}

	mMesh->Render();
	mTexture->ResetShader(0, (int)eBufferShaderTypeFlags::Pixel, 0);
}

CD2ShadowComponent* CD2ShadowComponent::Clone()
{
	return new CD2ShadowComponent(*this);
}

void CD2ShadowComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);

	fwrite(&mOffset, sizeof(Vector2), 1, fp);
}

void CD2ShadowComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);

	fread(&mOffset, sizeof(Vector2), 1, fp);
}
