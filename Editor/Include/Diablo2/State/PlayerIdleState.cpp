#include "PlayerIdleState.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2NavAgentComponent.h"
#include "PlayerRunState.h"
#include "PlayerWalkState.h"
#include "PlayerHitState.h"
#include "PlayerCastingState.h"
#include "GameObject/GameObject.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "Input.h"
#include "../D2Util.h"
#include "Component/ColliderComponent.h"

CPlayerIdleState::CPlayerIdleState()	:
	mbRun(false),
	mbCasting(false),
	mbMelee(false)
{
}

CPlayerIdleState::CPlayerIdleState(const CPlayerIdleState& state)	:
	CD2State(state)
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
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Push, this, &CPlayerIdleState::OnCtrlDown);
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Up, this, &CPlayerIdleState::OnCtrlUp);

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Idle" + std::to_string((int)spriteDir));
}

CState* CPlayerIdleState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	if (mbIsHit)
	{
		static_cast<CD2NavAgentComponent*>(mOwner->GetNavAgent())->CancleMove();
		return (CState*)(new CPlayerHitState);
	}
	else if (mbCasting)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
		Vector3 rootPos = mOwner->GetRootComponent()->GetWorldPos();
		Vector2 dir = mousePos - Vector2(rootPos.x, rootPos.y);
		dir.Normalize();

		state->GetCharInfo()->SetDir(dir);
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CPlayerCastingState);
	}
	else if (mOwner->GetNavAgent()->IsPathExist())
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
		Vector3 rootPos = mOwner->GetRootComponent()->GetWorldPos();
		Vector2 dir = mousePos - Vector2(rootPos.x, rootPos.y);

		dir.Normalize();

		if (mbRun)
		{
			state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());
			state->GetCharInfo()->SetDir(dir);
			return (CState*)(new CPlayerRunState);
		}
		else
		{
			state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed() / 2.f);
			state->GetCharInfo()->SetDir(dir);
			return (CState*)(new CPlayerWalkState);
		}
	}
	return nullptr;
}

void CPlayerIdleState::ExitStateFunction()
{
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Up);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
}

void CPlayerIdleState::ResetState()
{
	CD2State::ResetState();

	mbRun = false;
	mbMelee = false;
	mbCasting = false;
}

void CPlayerIdleState::OnClickMouseR(float deltaTime)
{
	// 이전 마우스 위치 저장
	CD2PlayerSkillComponent* com = static_cast<CD2StateComponent*>(mOwner)->GetSkill();
	static_cast<CD2StateComponent*>(mOwner)->SaveMousePos(CInput::GetInst()->GetMouseWorld2DPos());

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

void CPlayerIdleState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// 몬스터 공격에 맞은 경우, Hit상태로 변화
	switch (profile->Channel)
	{
	case eCollisionChannel::MonsterAttack:
		mbIsHit = true;
		break;
	}
}

void CPlayerIdleState::OnCollideExit(const CollisionResult& result)
{
}
