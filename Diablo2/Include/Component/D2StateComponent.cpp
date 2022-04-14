#include "D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "D2NavAgentComponent.h"
#include "D2EnemyNavAgentComponent.h"
#include "D2ShadowComponent.h"
#include "Component/ColliderBox2D.h"
#include "D2Inventory.h"
#include "../State/D2State.h"

CD2StateComponent::CD2StateComponent()	:
	mClickableObject(nullptr)
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
	mShadow = mObject->FindSceneComponentFromType<CD2ShadowComponent>();
	
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

	mCharInfo->SetEventCallBack("Die", this, &CD2StateComponent::OnDie, eD2CharInfoEventType::Die);
}

CD2StateComponent* CD2StateComponent::Clone()
{
	return new CD2StateComponent(*this);
}

void CD2StateComponent::OnDie()
{
	static_cast<CD2State*>(mStateStack.top())->OnDie();
}

void CD2StateComponent::OnCollideEnter(const CollisionResult& result)
{
	static_cast<CD2State*>(mStateStack.top())->OnCollideEnter(result);
}

void CD2StateComponent::OnCollideExit(const CollisionResult& result)
{
	static_cast<CD2State*>(mStateStack.top())->OnCollideExit(result);
}
