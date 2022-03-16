#include "PlayerWalkState.h"
#include "PlayerRunState.h"
#include "PlayerIdleState.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "Component/StateComponent.h"
#include "Component/SpriteComponent.h"
#include "../Component/D2NavAgentComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "../D2Util.h"
#include "PlayerCastingState.h"
#include "PlayerHitState.h"
#include "Component/ColliderComponent.h"

CPlayerWalkState::CPlayerWalkState()	:
	mbRun(false),
	mbWalkEnd(false),
	mbCasting(false),
	mPrevDir(eD2SpriteDir::S)
{
}

CPlayerWalkState::CPlayerWalkState(const CPlayerWalkState& state)	:
	CD2State(state)
{
	mbRun = false;
	mbWalkEnd = false;
	mbCasting = false;
	mPrevDir = eD2SpriteDir::S;
}

CPlayerWalkState::~CPlayerWalkState()
{
}

bool CPlayerWalkState::Init()
{
	return true;
}

void CPlayerWalkState::Start()
{
}

void CPlayerWalkState::PostUpdate(float deltaTime)
{
}

CPlayerWalkState* CPlayerWalkState::Clone()
{
	return new CPlayerWalkState(*this);
}

void CPlayerWalkState::EnterStateFunction()
{
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Down, this, &CPlayerWalkState::OnCtrlDown);
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Up, this, &CPlayerWalkState::OnCtrlUp);
	CInput::GetInst()->SetKeyCallBack("MouseLCtrl", eKeyState::KeyState_Push, this, &CPlayerWalkState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Push, this, &CPlayerWalkState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseR", eKeyState::KeyState_Push, this, &CPlayerWalkState::OnPushMouseR);

	mPrevDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Walk" + std::to_string((int)mPrevDir));

	mOwner->GetNavAgent()->SetFrameCallBack(this, &CPlayerWalkState::OnWalk);
	mOwner->GetNavAgent()->SetEndCallBack(this, &CPlayerWalkState::OnWalkEnd);
}

CState* CPlayerWalkState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	if (mbIsHit)
	{
		mbEnd = true;
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
		mbEnd = true;
		return (CState*)(new CPlayerCastingState);
	}
	if (mbRun)
	{
		mbEnd = true;
		state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());
		return (CState*)(new CPlayerRunState);
	}
	return nullptr;
}

void CPlayerWalkState::ExitStateFunction()
{
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Up);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Push);
	mOwner->GetNavAgent()->DeleteFrameCallBack();
	mOwner->GetNavAgent()->DeleteEndCallBack();
}

void CPlayerWalkState::ResetState()
{
	CD2State::ResetState();
	mbWalkEnd = false;
	mbRun = false;
}

void CPlayerWalkState::OnPushMouseR(float deltaTime)
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

void CPlayerWalkState::OnPushMouseL(float deltaTime)
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	mOwner->GetNavAgent()->Move(Vector3(mousePos.x, mousePos.y, 0.f));
}

void CPlayerWalkState::OnCtrlDown(float deltaTime)
{
	mbRun = true;
}

void CPlayerWalkState::OnCtrlUp(float deltaTime)
{
	mbRun = false;
}

void CPlayerWalkState::OnWalkEnd()
{
	mbEnd = true;
	mbWalkEnd = true;
}

void CPlayerWalkState::OnWalk()
{
	Vector3 direction = mOwner->GetNavAgent()->GetPathListFront() - mOwner->GetRootComponent()->GetWorldPos();
	direction.Normalize();
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetDir(Vector2(direction.x, direction.y));

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	if (mPrevDir != spriteDir)
	{
		mPrevDir = spriteDir;
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->ChangeAnimationKeepFrame("Walk" + std::to_string((int)mPrevDir));
	}
}

void CPlayerWalkState::OnCollideEnter(const CollisionResult& result)
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

void CPlayerWalkState::OnCollideExit(const CollisionResult& result)
{
}
