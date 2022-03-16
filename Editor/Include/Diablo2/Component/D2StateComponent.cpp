#include "D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "D2NavAgentComponent.h"
#include "D2EnemyNavAgentComponent.h"
#include "Component/ColliderBox2D.h"
#include "../State/D2State.h"

CD2StateComponent::CD2StateComponent()
{
	SetTypeID<CD2StateComponent>();
}

CD2StateComponent::CD2StateComponent(const CD2StateComponent& com)	:
	CStateComponent(com)
{
}

CD2StateComponent::~CD2StateComponent()
{
}

void CD2StateComponent::Start()
{
	CStateComponent::Start();

	mCharInfo = mObject->FindObjectComponentFromType<CD2CharacterInfoComponent>();
	mSkill = mObject->FindObjectComponentFromType<CD2PlayerSkillComponent>();
	
	if (!mSkill)
	{
		mEnemySkill = mObject->FindObjectComponentFromType<CD2EnemySkillComponent>();
	}

	mNavAgent = mObject->FindObjectComponentFromType<CD2NavAgentComponent>();
	
	// 현재 시스템에서 이렇게밖에 못 찾음
	if (!mNavAgent)
	{
		mNavAgent = mObject->FindObjectComponentFromType<CD2EnemyNavAgentComponent>();
	}

	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
}

void CD2StateComponent::OnCollideEnter(const CollisionResult& result)
{
	static_cast<CD2State*>(mStateStack.top())->OnCollideEnter(result);
}

void CD2StateComponent::OnCollideExit(const CollisionResult& result)
{
	static_cast<CD2State*>(mStateStack.top())->OnCollideExit(result);
}
