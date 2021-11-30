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
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}
