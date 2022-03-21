#include "D2AndarielDeadState.h"
#include "GameObject/GameObject.h"
#include "Component/DissolveComponent.h"
#include "../Component/D2StateComponent.h"
#include "Component/SpriteComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"

CD2AndarielDeadState::CD2AndarielDeadState()
{
}

CD2AndarielDeadState::CD2AndarielDeadState(const CD2AndarielDeadState& state)	:
	CD2State(state)
{
}

CD2AndarielDeadState::~CD2AndarielDeadState()
{
}

bool CD2AndarielDeadState::Init()
{
	return true;
}

void CD2AndarielDeadState::Start()
{
}

void CD2AndarielDeadState::PostUpdate(float deltaTime)
{
}

CD2AndarielDeadState* CD2AndarielDeadState::Clone()
{
	return new CD2AndarielDeadState(*this);
}

void CD2AndarielDeadState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "Dead" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->
		GetAnimationInstance()->SetEndCallBack(animName, this, &CD2AndarielDeadState::OnAnimEnd);
}

CState* CD2AndarielDeadState::StateFunction()
{
	return nullptr;
}

void CD2AndarielDeadState::ExitStateFunction()
{
}

void CD2AndarielDeadState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2AndarielDeadState::OnCollideExit(const CollisionResult& result)
{
}

void CD2AndarielDeadState::ResetState()
{
	CD2State::ResetState();
}

void CD2AndarielDeadState::OnAnimEnd()
{
	// TODO : 보스 사망 이벤트 처리
}
