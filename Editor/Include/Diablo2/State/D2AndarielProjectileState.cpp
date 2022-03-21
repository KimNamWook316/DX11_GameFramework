#include "D2AndarielProjectileState.h"
#include "../Component/D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "D2AndarielDieState.h"

CD2AndarielProjectileState::CD2AndarielProjectileState()
{
}

CD2AndarielProjectileState::CD2AndarielProjectileState(const CD2AndarielProjectileState& state)	:
	CD2State(state)
{
}

CD2AndarielProjectileState::~CD2AndarielProjectileState()
{
}

bool CD2AndarielProjectileState::Init()
{
	return true;
}

void CD2AndarielProjectileState::Start()
{
}

void CD2AndarielProjectileState::PostUpdate(float deltaTime)
{
}

CD2AndarielProjectileState* CD2AndarielProjectileState::Clone()
{
	return new CD2AndarielProjectileState(*this);
}

void CD2AndarielProjectileState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(0.f);

	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	Vector3 myPos = mOwner->GetGameObject()->GetWorldPos();
	Vector3 dir = playerPos - myPos;
	dir.Normalize();

	mDir = Vector2(dir.x, dir.y);
	state->GetCharInfo()->SetDir(mDir);

	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = "AndarielMeleeAttack" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);

	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2AndarielProjectileState::OnAnimEnd);

	mOwner->GetGameObject()->AddWorldScale(70.f, 70.f, 0.f);
	mOwner->GetGameObject()->SetPivot(0.5f, 0.2f, 0.f);
}

CState* CD2AndarielProjectileState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	if (mbDie)
	{
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2AndarielDieState);
	}
	return nullptr;
}

void CD2AndarielProjectileState::ExitStateFunction()
{
	mOwner->GetGameObject()->AddWorldScale(-70.f, -70.f, 0.f);
	mOwner->GetGameObject()->SetPivot(0.5f, 0.f, 0.f);
}

void CD2AndarielProjectileState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2AndarielProjectileState::OnCollideExit(const CollisionResult& result)
{
}

void CD2AndarielProjectileState::ResetState()
{
	CD2State::ResetState();
}

void CD2AndarielProjectileState::OnAnimEnd()
{
	static_cast<CD2StateComponent*>(mOwner)->GetEnemySkill()->DoSkill("AndarielProjectile",
		mOwner->GetGameObject()->GetWorldPos() + Vector3(0.f, 100.f, 0.f), Vector3::Zero, mDir);

	mbEnd = true;
}
