#include "D2SkeletonMoveState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "D2EnemyMoveState.h"
#include "D2SkeletonHitState.h"
#include "D2SkeletonAttackState.h"
#include "D2EnemyDieState.h"
#include "Component/ColliderComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CD2SkeletonMoveState::CD2SkeletonMoveState()	:
	mTimer(1.f),
	mDist(300.f)
{
}

CD2SkeletonMoveState::CD2SkeletonMoveState(const CD2SkeletonMoveState& state)	:
	CD2State(state)
{
}

CD2SkeletonMoveState::~CD2SkeletonMoveState()
{
}

bool CD2SkeletonMoveState::Init()
{
	return true;
}

void CD2SkeletonMoveState::Start()
{
}

CState* CD2SkeletonMoveState::Update(float deltaTime)
{
	mTimer += deltaTime;

	return CState::Update(deltaTime);
}

void CD2SkeletonMoveState::PostUpdate(float deltaTime)
{
}

CD2SkeletonMoveState* CD2SkeletonMoveState::Clone()
{
	return new CD2SkeletonMoveState(*this);
}

void CD2SkeletonMoveState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());

	mOwner->GetNavAgent()->SetFrameCallBack(this, &CD2SkeletonMoveState::OnMove);

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string name = mOwner->GetGameObject()->GetName();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(name + "Move" + std::to_string((int)spriteDir / 2));
}

CState* CD2SkeletonMoveState::StateFunction()
{
	// 플레이어와의 거리 측정
	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	float dist = playerPos.Distance(mOwner->GetGameObject()->GetWorldPos());

	if (mbDie)
	{
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2EnemyDieState);
	}
 	else if (dist <= mDist)
	{
		mbEnd = true;
		static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->CancleMove();
		return nullptr;
	}
	else if (mTimer >= 0.5f)
	{
		mTimer = 0.f;
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());
		static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->Move();
	}
	else if (mbIsHit)
	{
		return (CState*)(new CD2SkeletonHitState);
	}
	return nullptr;
}

void CD2SkeletonMoveState::ExitStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	mOwner->GetNavAgent()->DeleteFrameCallBack();
}

void CD2SkeletonMoveState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// 이동 상태에서 충돌하는 경우 : 플레이어 조우, 피격, 다른 몬스터와 충돌
	// 다른 몬스터와 충돌하는 경우, Idle 상태로 돌아가고, 이동은 잠깐 중단한다.
	switch (profile->Channel)
	{
	case eCollisionChannel::Player:
		mbAttack = true;
		break;
	case eCollisionChannel::PlayerAttack:
		mbIsHit = true;
		break;
	case eCollisionChannel::Monster:
	{
		Vector2 myDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetDir();
		Vector2 destDir = static_cast<CD2StateComponent*>(result.Dest->GetGameObject()->FindObjectComponentFromType<CD2StateComponent>())
			->GetCharInfo()->GetDir();

		// 진행 방향 상 앞을 가로막힌 경우에 다시 길을 찾는다.
		if ((myDir.x * destDir.x < 0) && (myDir.y * destDir.y < 0))
		{
			static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->MoveBlockedFront();
		}
		break;
	}
	}
}

void CD2SkeletonMoveState::OnCollideExit(const CollisionResult& result)
{
}

void CD2SkeletonMoveState::ResetState()
{
	CD2State::ResetState();

	mbAttack = false;
}

void CD2SkeletonMoveState::OnMove()
{
	Vector3 direction = mOwner->GetNavAgent()->GetPathListFront() - mOwner->GetRootComponent()->GetWorldPos();
	direction.Normalize();
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetDir(Vector2(direction.x, direction.y));

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	if (mPrevDir != spriteDir)
	{
		mPrevDir = spriteDir;
		std::string name = mOwner->GetGameObject()->GetName();
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->ChangeAnimation(name + "Move" + std::to_string((int)mPrevDir / 2));
	}
}
