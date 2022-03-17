#include "D2EnemyDieState.h"
#include "../Component/D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "D2EnemyDeadState.h"

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
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(0.f);
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "Die" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2EnemyDieState::OnAnimEnd);
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
	mbAnimEnd = true;
}
