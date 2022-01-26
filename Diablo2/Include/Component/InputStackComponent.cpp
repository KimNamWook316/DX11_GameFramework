#include "InputStackComponent.h"

CInputStackComponent::CInputStackComponent()	:
	mClearTimer(0.f)
{
}

CInputStackComponent::CInputStackComponent(const CInputStackComponent& com)	:
	CObjectComponent(com)
{
	*this = com;
	mClearTimer = 0.f;
}

CInputStackComponent::~CInputStackComponent()
{
}

bool CInputStackComponent::Init()
{
	if (!CObjectComponent::Init())
	{
		return false;
	}

	return true;
}

void CInputStackComponent::Update(float deltaTime)
{
	CObjectComponent::Update(deltaTime);

	mClearTimer += deltaTime;

	if (mClearTimer >= 1.f)
	{
		while (!mMouseInputStack.empty())
		{
			mMouseInputStack.pop();
		}
		while (!mKeyInputStack.empty())
		{
			mKeyInputStack.empty();
		}
		mClearTimer = 0.f;
	}
}

void CInputStackComponent::PostUpdate(float deltaTime)
{
	CObjectComponent::PostUpdate(deltaTime);
}

void CInputStackComponent::PrevRender()
{
	CObjectComponent::PrevRender();
}

void CInputStackComponent::Render()
{
	CObjectComponent::Render();
}

void CInputStackComponent::PostRender()
{
	CObjectComponent::PostRender();
}

CInputStackComponent* CInputStackComponent::Clone()
{
	return new CInputStackComponent(*this);
}
