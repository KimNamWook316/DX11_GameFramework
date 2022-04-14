#include "D2EnemyIdleState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "D2EnemyHitState.h"
#include "D2EnemyMoveState.h"
#include "D2EnemyMeleeAttackState.h"
#include "D2EnemyDieState.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/ColliderComponent.h"
#include "Component/SpriteComponent.h"

CD2EnemyIdleState::CD2EnemyIdleState()	:
	mDist(300.f),
	mbMove(false)
{
}

CD2EnemyIdleState::CD2EnemyIdleState(const CD2EnemyIdleState& state)	:
	CD2State(state)
{
	mDist = state.mDist;
	mbMove = state.mbMove;
}

CD2EnemyIdleState::~CD2EnemyIdleState()
{
}

bool CD2EnemyIdleState::Init()
{
	return true;
}

void CD2EnemyIdleState::Start()
{
}

void CD2EnemyIdleState::PostUpdate(float deltaTime)
{
}

CD2EnemyIdleState* CD2EnemyIdleState::Clone()
{
	return new CD2EnemyIdleState(*this);
}

void CD2EnemyIdleState::EnterStateFunction()
{
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string name = mOwner->GetGameObject()->GetName();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(name + "Idle" + std::to_string((int)spriteDir / 2));
}

CState* CD2EnemyIdleState::StateFunction()
{
	// 임시적으로 거리 측정해서 일정 거리 이상 되면 추적하도록
	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	float dist = playerPos.Distance(mOwner->GetGameObject()->GetWorldPos());

	if (dist <= mDist)
	{
		mbMove = true;
	}
	// Idle 상태에서 피격받은 경우, 바로 추적 상태로 전환
	else if (mbIsHit)
	{
		mbMove = true;
	}
	else
	{
		mbMove = false;
	}

	if (mbDie)
	{
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2EnemyDieState);
	}
	else if (mbMove)
	{
		return (CState*)(new CD2EnemyMoveState);
	}
	else if (mbIsHit)
	{
		return (CState*)(new CD2EnemyHitState);
	}
	else if (mbAttack)
	{
		return (CState*)(new CD2EnemyMeleeAttackState);
	}

	return nullptr;
}

void CD2EnemyIdleState::ExitStateFunction()
{
}

void CD2EnemyIdleState::OnCollideEnter(const CollisionResult& result)
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

void CD2EnemyIdleState::OnCollideExit(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// Wait 상태일 때 몬스터간 충돌에서 벗어난 경우, 다시 추적 시작한다.
	switch (profile->Channel)
	{
	case eCollisionChannel::Monster:
 //		if (static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->IsWaiting())
 //		{
 //			mbMove = true;
 //			static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->SetWait(false);
 //		}
		break;
	}
}

void CD2EnemyIdleState::ResetState()
{
	CD2State::ResetState();
}
