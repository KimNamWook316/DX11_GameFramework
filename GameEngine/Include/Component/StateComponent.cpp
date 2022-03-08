#include "StateComponent.h"
#include "State.h"
#include "SpriteComponent.h"
#include "NavAgentComponent.h"
#include "../GameObject/GameObject.h"
#include "../Scene/SceneManager.h"

CStateComponent::CStateComponent()	:
	mInitialStateType(-1)
{
	SetTypeID<CStateComponent>();
}

CStateComponent::CStateComponent(const CStateComponent& com)	:
	CObjectComponent(com)
{
	mInitialStateType = com.mInitialStateType;
	mStateStack.empty();
}

CStateComponent::~CStateComponent()
{
	Clear();
}

bool CStateComponent::Init()
{
	return true;
}

void CStateComponent::Start()
{
	mRoot = mObject->GetRootSceneComponent();
	mNavAgent = mObject->FindObjectComponentFromType<CNavAgentComponent>();
}

void CStateComponent::Update(float deltaTime)
{
	if (!mNavAgent)
	{
		mNavAgent = mObject->FindObjectComponentFromType<CNavAgentComponent>();
		return;
	}

	if (!mStateStack.empty())
	{
		CState* current = mStateStack.top();

		// ���� ���°� �����ٸ�, ���ÿ��� ����.
		if (current->IsEnd())
		{
			current->ExitStateFunction();
			SAFE_DELETE(current);
			mStateStack.pop();
			mStateStack.top()->EnterStateFunction();
			mStateStack.top()->Update(deltaTime);
			return;
		}

		// ���� ���°� �ٸ� ���·� ����Ǿ�� �Ѵٸ�, �ٸ� ���¸� push
		CState* next = current->Update(deltaTime);
		
		if (next)
		{
			mStateStack.top()->ExitStateFunction();
			mStateStack.top()->ResetState();
			mStateStack.push(next);
			next->SetOwner(this);
			next->Start();
			next->EnterStateFunction();
		}
	}
}

void CStateComponent::PostUpdate(float deltaTime)
{
}

void CStateComponent::PrevRender()
{
}

void CStateComponent::Render()
{
}

void CStateComponent::PostRender()
{
}

CStateComponent* CStateComponent::Clone()
{
	return new CStateComponent(*this);
}

void CStateComponent::Save(FILE* fp)
{
	CComponent::Save(fp);
	fwrite(&mInitialStateType, sizeof(size_t), 1, fp);
}

void CStateComponent::Load(FILE* fp)
{
	CComponent::Load(fp);
	fread(&mInitialStateType, sizeof(size_t), 1, fp);
	CSceneManager::GetInst()->CallCreateStateFunction(this, mInitialStateType);
}

void CStateComponent::Clear()
{
	while (!mStateStack.empty())
	{
		SAFE_DELETE(mStateStack.top());
		mStateStack.pop();
	}
}
