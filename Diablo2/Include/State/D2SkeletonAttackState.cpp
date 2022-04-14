#include "D2SkeletonAttackState.h"
#include "D2SkeletonHitState.h"
#include "D2EnemyDieState.h"
#include "D2SkeletonMoveState.h"
#include "Component/ColliderComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "../Component/D2EnemySkillComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CD2SkeletonAttackState::CD2SkeletonAttackState()	:
	mDist(300.f)
{
}

CD2SkeletonAttackState::CD2SkeletonAttackState(const CD2SkeletonAttackState& state)	:
	CD2State(state)
{
}

CD2SkeletonAttackState::~CD2SkeletonAttackState()
{
}

bool CD2SkeletonAttackState::Init()
{
	return true;
}

void CD2SkeletonAttackState::Start()
{
}

void CD2SkeletonAttackState::PostUpdate(float deltaTime)
{
}

CD2SkeletonAttackState* CD2SkeletonAttackState::Clone()
{
	return new CD2SkeletonAttackState(*this);
}

void CD2SkeletonAttackState::EnterStateFunction()
{
	Vector3 dir = mOwner->GetScene()->GetPlayerObj()->GetWorldPos() - mOwner->GetGameObject()->GetWorldPos();
	static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->SetDir(Vector2(dir.x, dir.y));
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "Attack" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetLoop(animName, true);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2SkeletonAttackState::OnAnimEnd);

	mOwner->GetScene()->GetResource()->SoundPlay("SkeletonAttack");
}

CState* CD2SkeletonAttackState::StateFunction()
{
	// 플레이어와의 거리 측정
	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	float dist = playerPos.Distance(mOwner->GetGameObject()->GetWorldPos());

	// 죽음
	if (mbDie)
	{
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2EnemyDieState);
	}
	// 피격
	else if (mbIsHit)
	{
		return (CState*)(new CD2SkeletonHitState);
	}
	else if (mbOutRange && mbAnimEnd)
	{
		return (CState*)(new CD2SkeletonMoveState);
	}
	// 플레이어와 사정거리 이상 멀어진 경우 이동
	else if (dist > mDist)
	{
		mbOutRange = true;
	}
	return nullptr;
}

void CD2SkeletonAttackState::ExitStateFunction()
{
}

void CD2SkeletonAttackState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// 공격 상태에서 피격당할 경우, 이 상태에서 바로 벗어난다.
	switch (profile->Channel)
	{
	case eCollisionChannel::PlayerAttack:
		mbIsHit = true;
		break;
	}
}

void CD2SkeletonAttackState::OnCollideExit(const CollisionResult& result)
{
}

void CD2SkeletonAttackState::ResetState()
{
	CD2State::ResetState();

	mbOutRange = false;
}

void CD2SkeletonAttackState::OnAnimEnd()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	Vector3 myPos = mOwner->GetGameObject()->GetWorldPos();
	Vector3 dir = playerPos - myPos;
	dir.Normalize();

	Vector2 dir2 = Vector2(dir.x, dir.y);
	state->GetCharInfo()->SetDir(dir2);

	static_cast<CD2StateComponent*>(mOwner)->GetEnemySkill()->DoSkill("SkeletonArrow",
		mOwner->GetGameObject()->GetWorldPos(), Vector3::Zero, dir2);

	if (mbOutRange)
	{
		mbAnimEnd = true;
	}
	else
	{
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetDir(dir2);

		eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();

		CSpriteComponent* sprite = (CSpriteComponent*)mOwner->GetRootComponent();
		std::string name = mOwner->GetGameObject()->GetName();
		std::string animName = mOwner->GetGameObject()->GetName() + "Attack" + std::to_string((int)spriteDir / 2);
		sprite->ChangeAnimation(animName);
		sprite->SetEndCallBack(animName, this, &CD2SkeletonAttackState::OnAnimEnd);
		sprite->GetAnimationInstance()->Replay();

		mOwner->GetScene()->GetResource()->SoundPlay("SkeletonAttack");
	}
}
