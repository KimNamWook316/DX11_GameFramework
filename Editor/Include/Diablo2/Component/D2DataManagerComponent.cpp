#include "D2DataManagerComponent.h"
#include "D2DataManager.h"

CD2DataManagerComponent::CD2DataManagerComponent()
{
	SetTypeID<CD2DataManagerComponent>();
}

CD2DataManagerComponent::~CD2DataManagerComponent()
{
	CD2DataManager::DestroyInst();
}

bool CD2DataManagerComponent::Init()
{
	CD2DataManager::GetInst()->Start();
	return true;
}

void CD2DataManagerComponent::Start()
{
}

void CD2DataManagerComponent::Update(float deltaTime)
{
}

void CD2DataManagerComponent::PostUpdate(float deltaTime)
{
}

void CD2DataManagerComponent::PrevRender()
{
}

void CD2DataManagerComponent::Render()
{
}

void CD2DataManagerComponent::PostRender()
{
}

CObjectComponent* CD2DataManagerComponent::Clone()
{
	return nullptr;
}

void CD2DataManagerComponent::Save(FILE* fp)
{
	CComponent::Save(fp);
}

void CD2DataManagerComponent::Load(FILE* fp)
{
	CComponent::Load(fp);
	CD2DataManager::GetInst()->Start();
}
