#include "D2AndarielSprayState.h"
#include "../Component/D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "D2AndarielDieState.h"

CD2AndarielSprayState::CD2AndarielSprayState()
{
}

CD2AndarielSprayState::CD2AndarielSprayState(const CD2AndarielSprayState& state)	:
	CD2State(state)
{
}

CD2AndarielSprayState::~CD2AndarielSprayState()
{
}

bool CD2AndarielSprayState::Init()
{
	return true;
}

void CD2AndarielSprayState::Start()
{
}

void CD2AndarielSprayState::PostUpdate(float deltaTime)
{
}

CD2AndarielSprayState* CD2AndarielSprayState::Clone()
{
	return new CD2AndarielSprayState(*this);
}

void CD2AndarielSprayState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(0.f);

	Vector3 playerPos = mOwner->GetScene()->GetPlayerObj()->GetWorldPos();
	Vector3 myPos = mOwner->GetGameObject()->GetWorldPos();
	Vector3 dir = playerPos - myPos;
	dir.Normalize();
	state->GetCharInfo()->SetDir(Vector2(dir.x, dir.y));

	Matrix matRot;
	matRot.Rotation(0.f, 0.f, -30.f);
	mSprayDir = dir.TransformCoord(matRot);

	mMatRot.Rotation(0.f, 0.f, 6.6f);

	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	mAnimName = "AndarielCast" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(mAnimName);

	// 5프레임부터 13프레임까지 공격
	for (int i = 5; i <= 13; ++i)
	{
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->GetAnimationInstance()->AddNotify(mAnimName, i, this, &CD2AndarielSprayState::OnAnimFrame);
	}

	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(mAnimName, this, &CD2AndarielSprayState::OnAnimEnd);
}

CState* CD2AndarielSprayState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	if (mbDie)
	{
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2AndarielDieState);
	}
	return nullptr;
}

void CD2AndarielSprayState::ExitStateFunction()
{
	for (int i = 5; i <= 13; ++i)
	{
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->GetAnimationInstance()->DeleteNotify(mAnimName);
	}
}

void CD2AndarielSprayState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2AndarielSprayState::OnCollideExit(const CollisionResult& result)
{
}

void CD2AndarielSprayState::ResetState()
{
	CD2State::ResetState();
}

void CD2AndarielSprayState::OnAnimFrame()
{
	static_cast<CD2StateComponent*>(mOwner)->GetEnemySkill()->DoSkill("AndarielSpray", mOwner->GetGameObject()->GetWorldPos()
		+ Vector3(0.f, 70.f, 0.f),
		Vector3::Zero, Vector2(mSprayDir.x, mSprayDir.y));

	mSprayDir = mSprayDir.TransformNormal(mMatRot);
}

void CD2AndarielSprayState::OnAnimEnd()
{
	mbEnd = true;
}
