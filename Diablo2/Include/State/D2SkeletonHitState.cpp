#include "D2SkeletonHitState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CD2SkeletonHitState::CD2SkeletonHitState()
{
}

CD2SkeletonHitState::CD2SkeletonHitState(const CD2SkeletonHitState& state)	:
	CD2State(state)
{
}

CD2SkeletonHitState::~CD2SkeletonHitState()
{
}

bool CD2SkeletonHitState::Init()
{
	return true;
}

void CD2SkeletonHitState::Start()
{
}

void CD2SkeletonHitState::PostUpdate(float deltaTime)
{
}

CD2SkeletonHitState* CD2SkeletonHitState::Clone()
{
	return new CD2SkeletonHitState(*this);
}

void CD2SkeletonHitState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	mPrevSpeed = state->GetCharInfo()->GetSpeed();
	state->GetCharInfo()->SetSpeed(0.f);
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	mAnimName = mOwner->GetGameObject()->GetName() + "Hit" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(mAnimName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(mAnimName, this, &CD2SkeletonHitState::OnAnimEnd);
	mOwner->GetRootComponent()->AddWorldScale(-23.f, -10.f, 0.f);

	mOwner->GetScene()->GetResource()->SoundPlay("SkeletonHit");
}

CState* CD2SkeletonHitState::StateFunction()
{
	return nullptr;
}

void CD2SkeletonHitState::ExitStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(mPrevSpeed);
	mOwner->GetRootComponent()->AddWorldScale(23.f, 10.f, 0.f);
}

void CD2SkeletonHitState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2SkeletonHitState::OnCollideExit(const CollisionResult& result)
{
}

void CD2SkeletonHitState::ResetState()
{
	CD2State::ResetState();
}

void CD2SkeletonHitState::OnAnimEnd()
{
	mbEnd = true;
}
