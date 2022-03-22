#include "D2UIManagerComponent.h"
#include "D2UIManager.h"

CD2UIManagerComponent::CD2UIManagerComponent()
{
	SetTypeID<CD2UIManagerComponent>();
}

CD2UIManagerComponent::CD2UIManagerComponent(const CD2UIManagerComponent& com) :
	CObjectComponent(com)
{
}

CD2UIManagerComponent::~CD2UIManagerComponent()
{
}

bool CD2UIManagerComponent::Init()
{
	return true;
}

void CD2UIManagerComponent::Start()
{
	CObjectComponent::Start();
	CD2UIManager::GetInst()->Start();
}

void CD2UIManagerComponent::Update(float deltaTime)
{
}

void CD2UIManagerComponent::PostUpdate(float deltaTime)
{
}

void CD2UIManagerComponent::PrevRender()
{
}

void CD2UIManagerComponent::Render()
{
}

void CD2UIManagerComponent::PostRender()
{
}

CD2UIManagerComponent* CD2UIManagerComponent::Clone()
{
	return nullptr;
}
