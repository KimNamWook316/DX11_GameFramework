#include "D2ItemTableComponent.h"
#include "D2ItemTable.h"

CD2ItemTableComponent::CD2ItemTableComponent()
{
}

CD2ItemTableComponent::CD2ItemTableComponent(const CD2ItemTableComponent& com)
{
}

CD2ItemTableComponent::~CD2ItemTableComponent()
{
}

bool CD2ItemTableComponent::Init()
{
	return true;
}

void CD2ItemTableComponent::Start()
{
	CD2ItemTable::GetInst()->Start();
}

void CD2ItemTableComponent::Update(float deltaTime)
{
}

void CD2ItemTableComponent::PostUpdate(float deltaTime)
{
}

void CD2ItemTableComponent::PrevRender()
{
}

void CD2ItemTableComponent::Render()
{
}

void CD2ItemTableComponent::PostRender()
{
}

CD2ItemTableComponent* CD2ItemTableComponent::Clone()
{
	return nullptr;
}
