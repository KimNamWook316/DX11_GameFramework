#include "D2ItemTableComponent.h"
#include "D2ItemTable.h"

CD2ItemTableComponent::CD2ItemTableComponent()
{
	SetTypeID<CD2ItemTableComponent>();
}

CD2ItemTableComponent::CD2ItemTableComponent(const CD2ItemTableComponent& com)	:
	CObjectComponent(com)
{
}

CD2ItemTableComponent::~CD2ItemTableComponent()
{
	CD2ItemTable::GetInst()->DestroyInst();
}

bool CD2ItemTableComponent::Init()
{
	return true;
}

void CD2ItemTableComponent::Start()
{
}

void CD2ItemTableComponent::Update(float deltaTime)
{
	if (!mbInit)
	{
		CD2ItemTable::GetInst()->Start();
		mbInit = true;
	}
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
	return new CD2ItemTableComponent(*this);
}
