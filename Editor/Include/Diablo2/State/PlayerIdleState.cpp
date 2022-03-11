#include "PlayerIdleState.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "../Component/D2StateComponent.h"
#include "PlayerRunState.h"
#include "PlayerWalkState.h"
#include "PlayerCastingState.h"
#include "GameObject/GameObject.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "Input.h"
#include "../D2Util.h"

CPlayerIdleState::CPlayerIdleState()	:
	mbRun(false),
	mbCasting(false),
	mbMelee(false)
{
}

CPlayerIdleState::CPlayerIdleState(const CPlayerIdleState& state)	:
	CState(state)
{
	mbRun = false;
	mbCasting = false;
	mbMelee = false;
}

CPlayerIdleState::~CPlayerIdleState()
{
}

bool CPlayerIdleState::Init()
{
	return true;
}

void CPlayerIdleState::Start()
{
}

void CPlayerIdleState::PostUpdate(float deltaTime)
{
}

CState* CPlayerIdleState::Clone()
{
	return new CPlayerIdleState(*this);
}

void CPlayerIdleState::EnterStateFunction()
{
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Push, this, &CPlayerIdleState::OnClickMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseLCtrl", eKeyState::KeyState_Push, this, &CPlayerIdleState::OnClickMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseR", eKeyState::KeyState_Down, this, &CPlayerIdleState::OnClickMouseR);
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Down, this, &CPlayerIdleState::OnCtrlDown);
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Up, this, &CPlayerIdleState::OnCtrlUp);

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Idle" + std::to_string((int)spriteDir));
}

CState* CPlayerIdleState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	if (mbCasting)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
		Vector3 rootPos = mOwner->GetRootComponent()->GetWorldPos();
		Vector2 dir = mousePos - Vector2(rootPos.x, rootPos.y);
		dir.Normalize();

		state->GetCharInfo()->SetDir(dir);
		state->GetCharInfo()->SetSpeed(0.f);
		mOwner->GetNavAgent()->SetMoveSpeed(state->GetCharInfo()->GetSpeed());
		return (CState*)(new CPlayerCastingState);
	}
	else if (mOwner->GetNavAgent()->IsPathExist())
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
		Vector3 rootPos = mOwner->GetRootComponent()->GetWorldPos();
		Vector2 dir = mousePos - Vector2(rootPos.x, rootPos.y);

		dir.Normalize();
		state->GetCharInfo()->SetDir(dir);

		if (mbRun)
		{
			state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());
			mOwner->GetNavAgent()->SetMoveSpeed(state->GetCharInfo()->GetSpeed());
			return (CState*)(new CPlayerRunState);
		}
		else
		{
			state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed() / 2.f);
			mOwner->GetNavAgent()->SetMoveSpeed(state->GetCharInfo()->GetSpeed());
			return (CState*)(new CPlayerWalkState);
		}
	}
	return nullptr;
}

void CPlayerIdleState::ExitStateFunction()
{
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Up);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
}

void CPlayerIdleState::ResetState()
{
	CState::ResetState();

	mbRun = false;
	mbMelee = false;
	mbCasting = false;
}

void CPlayerIdleState::OnClickMouseR(float deltaTime)
{
	CD2PlayerSkillComponent* com = static_cast<CD2StateComponent*>(mOwner)->GetSkill();

	if ((int)(eD2AttackType::Projectile) == com->GetRSkillType())
	{
		mbCasting = true;
	}
	else if ((int)(eD2AttackType::Melee) == com->GetRSkillType())
	{
	}
	else if ((int)(eD2AttackType::Casting) == com->GetRSkillType())
	{
		mbCasting = true;
	}
}

void CPlayerIdleState::OnClickMouseL(float deltaTime)
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	mOwner->GetNavAgent()->Move(Vector3(mousePos.x, mousePos.y, 0.f));
}

void CPlayerIdleState::OnCtrlDown(float deltaTime)
{
	mbRun = true;
}

void CPlayerIdleState::OnCtrlUp(float deltaTime)
{
	mbRun = false;
}
