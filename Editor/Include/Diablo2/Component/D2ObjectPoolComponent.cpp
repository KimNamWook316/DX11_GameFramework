#include "D2ObjectPoolComponent.h"
#include "D2ObjectPool.h"

CD2ObjectPoolComponent::CD2ObjectPoolComponent()
{
	SetTypeID<CD2ObjectPoolComponent>();
}

CD2ObjectPoolComponent::CD2ObjectPoolComponent(const CD2ObjectPoolComponent& com)	:
	CObjectComponent(com)
{
}

CD2ObjectPoolComponent::~CD2ObjectPoolComponent()
{
}

bool CD2ObjectPoolComponent::Init()
{
	return true;
}

void CD2ObjectPoolComponent::Start()
{
	CObjectComponent::Start();
	CD2ObjectPool::GetInst()->Start();
}

void CD2ObjectPoolComponent::Update(float deltaTime)
{
}

void CD2ObjectPoolComponent::PostUpdate(float deltaTime)
{
}

void CD2ObjectPoolComponent::PrevRender()
{
}

void CD2ObjectPoolComponent::Render()
{
}

void CD2ObjectPoolComponent::PostRender()
{
}

CD2ObjectPoolComponent* CD2ObjectPoolComponent::Clone()
{
	return new CD2ObjectPoolComponent(*this);
}
