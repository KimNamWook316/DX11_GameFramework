#include "D2AndarielIdleState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "D2EnemyDeadState.h"
#include "D2AndarielMoveState.h"
#include "D2AndarielMeleeAttackState.h"
#include "D2AndarielDieState.h"
#include "D2EnemyHitState.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/ColliderComponent.h"
#include "Component/SpriteComponent.h"

CD2AndarielIdleState::CD2AndarielIdleState()	:
	mDist(300.f),
	mbMove(false)
{
}

CD2AndarielIdleState::CD2AndarielIdleState(const CD2AndarielIdleState& state)	:
	CD2State(state)
{
	mDist = state.mDist;
	mbMove = state.mbMove;
}

CD2AndarielIdleState::~CD2AndarielIdleState()
{
}

bool CD2AndarielIdleState::Init()
{
	return true;
}

void CD2AndarielIdleState::Start()
{
}

void CD2AndarielIdleState::PostUpdate(float deltaTime)
{
}

CD2AndarielIdleState* CD2AndarielIdleState::Clone()
{
	return new CD2AndarielIdleState(*this);
}

void CD2AndarielIdleState::EnterStateFunction()
{
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string name = mOwner->GetGameObject()->GetName();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(name + "Idle" + std::to_string((int)spriteDir / 2));
}

CState* CD2AndarielIdleState::StateFunction()
{
	// �ӽ������� �Ÿ� �����ؼ� ���� �Ÿ� �̻� �Ǹ� �����ϵ���
	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	float dist = playerPos.Distance(mOwner->GetGameObject()->GetWorldPos());

	if (dist <= mDist)
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
		return (CState*)(new CD2AndarielDieState);
	}
	else if (mbIsHit)
	{
		return (CState*)(new CD2EnemyHitState);
	}
	else if (mbMove)
	{
		return (CState*)(new CD2AndarielMoveState);
	}
	else if (mbAttack)
	{
		return (CState*)(new CD2AndarielMeleeAttackState);
	}

	return nullptr;
}

void CD2AndarielIdleState::ExitStateFunction()
{
}

void CD2AndarielIdleState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// Idle ���¿��� �浹�ϴ� ��� : �÷��̾� ����, �������̴� ���Ϳ� �浹�� ���, �÷��̾� ���ݰ� �浹�� ���
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

void CD2AndarielIdleState::OnCollideExit(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// Wait ������ �� ���Ͱ� �浹���� ��� ���, �ٽ� ���� �����Ѵ�.
	switch (profile->Channel)
	{
	case eCollisionChannel::Monster:
		if (static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->IsWaiting())
		{
			mbMove = true;
			static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->SetWait(false);
		}
		break;
	}
}

void CD2AndarielIdleState::ResetState()
{
	CD2State::ResetState();
}
