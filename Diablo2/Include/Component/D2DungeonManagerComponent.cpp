#include "D2DungeonManagerComponent.h"
#include "D2DungeonManager.h"

CD2DungeonManagerComponent::CD2DungeonManagerComponent()
{
	SetTypeID<CD2DungeonManagerComponent>();
}

CD2DungeonManagerComponent::CD2DungeonManagerComponent(const CD2DungeonManagerComponent& com)	:
	CObjectComponent(com)
{
}

CD2DungeonManagerComponent::~CD2DungeonManagerComponent()
{
	CD2DungeonManager::GetInst()->DestroyInst();
}

bool CD2DungeonManagerComponent::Init()
{
	return true;
}

void CD2DungeonManagerComponent::Start()
{
	CObjectComponent::Start();
}

void CD2DungeonManagerComponent::Update(float deltaTime)
{
	if (!mbInit)
	{
		CD2DungeonManager::GetInst()->Start();
		mbInit = true;
	}
}

void CD2DungeonManagerComponent::PostUpdate(float deltaTime)
{
}

void CD2DungeonManagerComponent::PrevRender()
{
}

void CD2DungeonManagerComponent::Render()
{
}

void CD2DungeonManagerComponent::PostRender()
{
}

CD2DungeonManagerComponent* CD2DungeonManagerComponent::Clone()
{
	return nullptr;
}
