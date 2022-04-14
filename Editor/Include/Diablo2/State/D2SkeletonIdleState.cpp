#include "D2SkeletonIdleState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "D2SkeletonHitState.h"
#include "D2SkeletonMoveState.h"
#include "D2SkeletonAttackState.h"
#include "D2EnemyDieState.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/ColliderComponent.h"
#include "Component/SpriteComponent.h"

CD2SkeletonIdleState::CD2SkeletonIdleState()	:
	mDist(300.f)
{
}

CD2SkeletonIdleState::CD2SkeletonIdleState(const CD2SkeletonIdleState& state)	:
	CD2State(state)
{
	mDist = state.mDist;
}

CD2SkeletonIdleState::~CD2SkeletonIdleState()
{
}

bool CD2SkeletonIdleState::Init()
{
	return true;
}

void CD2SkeletonIdleState::Start()
{
}

void CD2SkeletonIdleState::PostUpdate(float deltaTime)
{
}

CD2SkeletonIdleState* CD2SkeletonIdleState::Clone()
{
	return new CD2SkeletonIdleState(*this);
}

void CD2SkeletonIdleState::EnterStateFunction()
{
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string name = mOwner->GetGameObject()->GetName();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(name + "Idle" + std::to_string((int)spriteDir / 2));
}

CState* CD2SkeletonIdleState::StateFunction()
{
	// 거리 측정해서 일정 거리 이상 되면 공격 
	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	float dist = playerPos.Distance(mOwner->GetGameObject()->GetWorldPos());

	if (dist <= mDist)
	{
		mbAttack = true;
	}
	else
	{
		mbAttack = false;
	}

	if (mbDie)
	{
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2EnemyDieState);
	}
	else if (mbIsHit)
	{
		return (CState*)(new CD2SkeletonHitState);
	}
	else if (mbAttack)
	{
		return (CState*)(new CD2SkeletonAttackState);
	}

	return nullptr;
}

void CD2SkeletonIdleState::ExitStateFunction()
{
}

void CD2SkeletonIdleState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// Idle 상태에서 충돌하는 경우 : 플레이어 조우, 추적중이던 몬스터와 충돌한 경우, 플레이어 공격과 충돌한 경우
	switch (profile->Channel)
	{
	case eCollisionChannel::Player:
		mbAttack = true;
		break;
	case eCollisionChannel::PlayerAttack:
		mbIsHit = true;
		break;
	}
}

void CD2SkeletonIdleState::OnCollideExit(const CollisionResult& result)
{
}

void CD2SkeletonIdleState::ResetState()
{
	CD2State::ResetState();
}
