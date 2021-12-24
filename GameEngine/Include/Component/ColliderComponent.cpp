#include "ColliderComponent.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
}

CColliderComponent::~CColliderComponent()
{
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		assert(false);
		return false;
	}
	return true;
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

void CColliderComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CColliderComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColliderComponent::Render()
{
	CSceneComponent::Render();
}

void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderComponent* CColliderComponent::Clone()
{
	return new CColliderComponent(*this);
}

void CColliderComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);
}

void CColliderComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
}
