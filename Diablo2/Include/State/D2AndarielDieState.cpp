#include "D2AndarielDieState.h"
#include "../Component/D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "D2AndarielDeadState.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "../Component/D2DungeonManager.h"
#include "Scene/Scene.h"

CD2AndarielDieState::CD2AndarielDieState()	:
	mShakeCameraTimer(0.f)
{
}

CD2AndarielDieState::CD2AndarielDieState(const CD2AndarielDieState& state)	:
	CD2State(state)
{
}

CD2AndarielDieState::~CD2AndarielDieState()
{
}

bool CD2AndarielDieState::Init()
{
	return true;
}

void CD2AndarielDieState::Start()
{
}

CState* CD2AndarielDieState::Update(float deltaTime)
{
	mShakeCameraTimer += deltaTime;

	return CState::Update(deltaTime);
}

void CD2AndarielDieState::PostUpdate(float deltaTime)
{
}

CD2AndarielDieState* CD2AndarielDieState::Clone()
{
	return new CD2AndarielDieState(*this);
}

void CD2AndarielDieState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(0.f);
	static_cast<CD2EnemyNavAgentComponent*>(state->GetNavAgent())->CancleMove();
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "Die" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	state->GetCollider()->Enable(false);
	mOwner->GetScene()->GetResource()->SoundPlay("AndarielDeath");
}

CState* CD2AndarielDieState::StateFunction()
{
	// TODO : Camera Shake 효과
	if (mShakeCameraTimer >= 5.f)
	{
		// 플레이어 경험치
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		CD2DungeonManager::GetInst()->OnMonsterDie(state->GetCharInfo()->GetMaxExp());
		return (CState*)(new CD2AndarielDeadState);
	}

	return nullptr;
}

void CD2AndarielDieState::ExitStateFunction()
{
}

void CD2AndarielDieState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2AndarielDieState::OnCollideExit(const CollisionResult& result)
{
}

void CD2AndarielDieState::ResetState()
{
	CD2State::ResetState();
}
