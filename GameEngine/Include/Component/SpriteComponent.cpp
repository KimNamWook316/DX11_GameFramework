#include "SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CSpriteComponent::CSpriteComponent()
{
	SetTypeID<CSpriteComponent>();
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com)
	: CSceneComponent(com)
{
	mMesh = com.mMesh;
}

CSpriteComponent::~CSpriteComponent()
{
}

bool CSpriteComponent::Init()
{
	mMesh = (CSpriteMesh*)mScene->GetResource()->FindMesh("SpriteMesh");
	mMaterial = mScene->GetResource()->FindMaterial("Color");
	return true;
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
	CSceneComponent::Render();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}
