#include "InputStackComponent.h"

CInputStackComponent::CInputStackComponent()	:
	mClearTimer(0.f)
{
}

CInputStackComponent::CInputStackComponent(const CInputStackComponent& com)	:
	CSceneComponent(com)
{
	*this = com;
	mClearTimer = 0.f;
}

CInputStackComponent::~CInputStackComponent()
{
}

bool CInputStackComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}

	return true;
}

void CInputStackComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	mClearTimer += deltaTime;

	if (mClearTimer >= 1.f)
	{
		while (!mMouseInputStack.empty())
		{
			mMouseInputStack.pop();
		}
		while (!mKeyInputStack.empty())
		{
			mKeyInputStack.pop();
		}
		mClearTimer = 0.f;
	}
}

void CInputStackComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CInputStackComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CInputStackComponent::Render()
{
	CSceneComponent::Render();
}

void CInputStackComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CInputStackComponent* CInputStackComponent::Clone()
{
	return new CInputStackComponent(*this);
}
