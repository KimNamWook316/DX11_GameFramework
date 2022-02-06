#include "DissolveComponent.h"

CDissolveComponent::CDissolveComponent()
{
	mCBuffer = new CDissolveConstantBuffer;
	mCBuffer->Init();
}

CDissolveComponent::CDissolveComponent(CDissolveComponent& com)	:
	CObjectComponent(com)
{
	mCBuffer = new CDissolveConstantBuffer;
	mCBuffer->Init();
}

CDissolveComponent::~CDissolveComponent()
{
}

bool CDissolveComponent::Init()
{
	return true;
}

void CDissolveComponent::Start()
{
	CObjectComponent::Start();
}

void CDissolveComponent::Update(float deltaTime)
{
}

void CDissolveComponent::PostUpdate(float deltaTime)
{
}

void CDissolveComponent::PrevRender()
{
}

void CDissolveComponent::Render()
{
}

void CDissolveComponent::PostRender()
{
}

CDissolveComponent* CDissolveComponent::Clone()
{
	return new CDissolveComponent(*this);
}
