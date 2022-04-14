#include "PlayerRunState.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "../Component/D2NavAgentComponent.h"
#include "PlayerWalkState.h"
#include "PlayerCastingState.h"
#include "PlayerHitState.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "../D2Util.h"
#include "Component/ColliderComponent.h"
#include "Scene/Scene.h"

CPlayerRunState::CPlayerRunState()	:
	mbRunEnd(false),
	mbCasting(false),
	mbWalk(false),
	mPrevDir(eD2SpriteDir::S)
{
}

CPlayerRunState::CPlayerRunState(const CPlayerRunState& state)	:
	CD2State(state)
{
}

CPlayerRunState::~CPlayerRunState()
{
}

bool CPlayerRunState::Init()
{
	return true;
}

void CPlayerRunState::Start()
{
}

void CPlayerRunState::PostUpdate(float deltaTime)
{
}

CState* CPlayerRunState::Clone()
{
	return new CPlayerRunState(*this);
}

void CPlayerRunState::EnterStateFunction()
{
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Up, this, &CPlayerRunState::OnCtrlUp);
	CInput::GetInst()->SetKeyCallBack("MouseLCtrl", eKeyState::KeyState_Push, this, &CPlayerRunState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Push, this, &CPlayerRunState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseRCtrl", eKeyState::KeyState_Down, this, &CPlayerRunState::OnDownMouseRCtrl);

	mPrevDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Run" + std::to_string((int)mPrevDir));

	mOwner->GetNavAgent()->SetFrameCallBack(this, &CPlayerRunState::OnRun);
	mOwner->GetNavAgent()->SetEndCallBack(this, &CPlayerRunState::OnRunEnd);

	// Sound
	mOwner->GetScene()->GetResource()->SoundPlay("PlayerRun");
}

CState* CPlayerRunState::StateFunction()
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
	else if (mbRunEnd)
	{
		mbEnd = true;
		return nullptr;
	}
	else if (mbWalk)
	{
		mbEnd = true;
		state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed() / 2.f);
		return (CState*)(new CPlayerWalkState);
	}
	return nullptr;
}

void CPlayerRunState::ExitStateFunction()
{
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Up);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseRCtrl", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Push);
	mOwner->GetNavAgent()->DeleteFrameCallBack();
	mOwner->GetNavAgent()->DeleteEndCallBack();
	
	// Sound
	mOwner->GetScene()->GetResource()->SoundStop("PlayerRun");
}

void CPlayerRunState::ResetState()
{
	CD2State::ResetState();
	mbRunEnd = false;
	mbWalk = false;
	mbCasting = false;
}

void CPlayerRunState::OnDownMouseRCtrl(float deltaTime)
{
	// 이전 마우스 위치 저장
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	CD2PlayerSkillComponent* com = state->GetSkill();
	state->SaveMousePos(CInput::GetInst()->GetMouseWorld2DPos());

	if ((int)(eD2AttackType::Projectile) == com->GetRSkillType())
	{
		float curMp = state->GetCharInfo()->GetMp();
		float skillMp = com->GetRSkillMp();

		if (curMp >= skillMp)
		{
			state->GetCharInfo()->SetMp(-skillMp);
			
			float range = com->GetRSkillRange();
			Vector2 mousePos = state->GetPrevStateMousePos();
			float dist = mOwner->GetGameObject()->GetWorldPos().Distance(Vector3(mousePos.x, mousePos.y, 0.f));

			mbCasting = true;

			// 범위 이내면 지금 마우스 위치에 스킬 시전 
			if (dist <= range)
			{
				state->ReserveSkillTargetPos(Vector3(mousePos.x, mousePos.y, 0.f));
			}
			// 범위를 벗어난다면, 최대 범위에 시전
			else
			{
				Vector3 targetPos = Vector3(mousePos.x, mousePos.y, 0.f) - mOwner->GetGameObject()->GetWorldPos();
				targetPos.Normalize();
				targetPos *= range;
				targetPos += mOwner->GetGameObject()->GetWorldPos();
				state->ReserveSkillTargetPos(targetPos);
			}
		}
	}
	else if ((int)(eD2AttackType::Melee) == com->GetRSkillType())
	{
	}
	else if ((int)(eD2AttackType::Casting) == com->GetRSkillType())
	{
		float curMp = state->GetCharInfo()->GetMp();
		float skillMp = com->GetRSkillMp();

		if (curMp >= skillMp)
		{
			state->GetCharInfo()->SetMp(-skillMp);

			float range = com->GetRSkillRange();
			Vector2 mousePos = state->GetPrevStateMousePos();
			float dist = mOwner->GetGameObject()->GetWorldPos().Distance(Vector3(mousePos.x, mousePos.y, 0.f));

			mbCasting = true;

			if (dist <= range)
			{
				state->ReserveSkillTargetPos(Vector3(mousePos.x, mousePos.y, 0.f));
			}
			// 범위를 벗어난다면, 최대 범위에 시전
			else
			{
				Vector3 targetPos = Vector3(mousePos.x, mousePos.y, 0.f) - mOwner->GetGameObject()->GetWorldPos();
				targetPos.Normalize();
				targetPos *= range;
				targetPos += mOwner->GetGameObject()->GetWorldPos();
				state->ReserveSkillTargetPos(targetPos);
			}
		}
	}
}

void CPlayerRunState::OnPushMouseL(float deltaTime)
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	mOwner->GetNavAgent()->Move(Vector3(mousePos.x, mousePos.y, 0.f));
}

void CPlayerRunState::OnCtrlUp(float delatTime)
{
	mbWalk = true;
}

void CPlayerRunState::OnRunEnd()
{
	mbRunEnd = true;
}

void CPlayerRunState::OnRun()
{
	Vector3 direction = mOwner->GetNavAgent()->GetPathListFront() - mOwner->GetRootComponent()->GetWorldPos();
	direction.Normalize();
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetDir(Vector2(direction.x, direction.y));

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	if (mPrevDir != spriteDir)
	{
		mPrevDir = spriteDir;
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->ChangeAnimationKeepFrame("Run" + std::to_string((int)mPrevDir));
	}
}

void CPlayerRunState::OnCollideEnter(const CollisionResult& result)
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

void CPlayerRunState::OnCollideExit(const CollisionResult& result)
{
}
