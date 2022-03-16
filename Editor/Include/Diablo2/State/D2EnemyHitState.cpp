#include "D2EnemyHitState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"

CD2EnemyHitState::CD2EnemyHitState()
{
}

CD2EnemyHitState::CD2EnemyHitState(const CD2EnemyHitState& state)	:
	CD2State(state)
{
}

CD2EnemyHitState::~CD2EnemyHitState()
{
}

bool CD2EnemyHitState::Init()
{
	return true;
}

void CD2EnemyHitState::Start()
{
}

void CD2EnemyHitState::PostUpdate(float deltaTime)
{
}

CD2EnemyHitState* CD2EnemyHitState::Clone()
{
	return new CD2EnemyHitState(*this);
}

void CD2EnemyHitState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	mPrevSpeed = state->GetCharInfo()->GetSpeed();
	state->GetCharInfo()->SetSpeed(0.f);
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	mAnimName = mOwner->GetGameObject()->GetName() + "Hit" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(mAnimName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(mAnimName, this, &CD2EnemyHitState::OnAnimEnd);
}

CState* CD2EnemyHitState::StateFunction()
{
	return nullptr;
}

void CD2EnemyHitState::ExitStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(mPrevSpeed);
}

void CD2EnemyHitState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2EnemyHitState::OnCollideExit(const CollisionResult& result)
{
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->DeleteCallBack(mAnimName);
}

void CD2EnemyHitState::ResetState()
{
	CD2State::ResetState();
}

void CD2EnemyHitState::OnAnimEnd()
{
	mbEnd = true;
}
