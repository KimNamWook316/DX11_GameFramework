#include "D2EnemyDieState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "Component/ColliderBox2D.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "D2EnemyDeadState.h"
#include "../Component/D2ShadowComponent.h"
#include "../Component/D2DungeonManager.h"
#include "Scene/Scene.h"

CD2EnemyDieState::CD2EnemyDieState()
{
}

CD2EnemyDieState::CD2EnemyDieState(const CD2EnemyDieState& state)	:
	CD2State(state)
{
}

CD2EnemyDieState::~CD2EnemyDieState()
{
}

bool CD2EnemyDieState::Init()
{
	return true;
}

void CD2EnemyDieState::Start()
{
}

void CD2EnemyDieState::PostUpdate(float deltaTime)
{
}

CD2EnemyDieState* CD2EnemyDieState::Clone()
{
	return new CD2EnemyDieState(*this);
}

void CD2EnemyDieState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(0.f);
	static_cast<CD2EnemyNavAgentComponent*>(state->GetNavAgent())->CancleMove();
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "Die" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2EnemyDieState::OnAnimEnd);
	state->GetCollider()->Enable(false);
	state->GetShadow()->Enable(false);

	if (mOwner->GetGameObject()->GetName() == "Barlog")
	{
		mOwner->GetScene()->GetResource()->SoundPlay("DemonDeath");
	}
	else if (mOwner->GetGameObject()->GetName() == "Skeleton")
	{
		mOwner->GetScene()->GetResource()->SoundPlay("SkeletonDeath");
	}
}

CState* CD2EnemyDieState::StateFunction()
{
	if (mbAnimEnd)
	{
		return (CState*)(new CD2EnemyDeadState);
	}

	return nullptr;
}

void CD2EnemyDieState::ExitStateFunction()
{
}

void CD2EnemyDieState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2EnemyDieState::OnCollideExit(const CollisionResult& result)
{
}

void CD2EnemyDieState::ResetState()
{
}

void CD2EnemyDieState::OnAnimEnd()
{
	if (!mbAnimEnd)
	{
		// 플레이어 경험치
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		CD2DungeonManager::GetInst()->OnMonsterDie(state->GetCharInfo()->GetMaxExp());
	}

	mbAnimEnd = true;
	
}
