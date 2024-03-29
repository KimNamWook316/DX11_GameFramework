#include "PlayerIdleState.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2NavAgentComponent.h"
#include "../Component/D2ItemObjectComponent.h"
#include "../Component/D2PortalObject.h"
#include "PlayerRunState.h"
#include "PlayerWalkState.h"
#include "PlayerHitState.h"
#include "PlayerCastingState.h"
#include "GameObject/GameObject.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "Input.h"
#include "../D2Util.h"
#include "Component/ColliderComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneCollision.h"
#include "../Component/D2Inventory.h"
#include "Engine.h"
#include "../Component/D2UIManager.h"
#include "../UI/D2InventoryWindow.h"

CPlayerIdleState::CPlayerIdleState()	:
	mbRun(false),
	mbCasting(false),
	mbMelee(false),
	mRecoveryTimer(0.f)
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
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Up);
}

bool CPlayerIdleState::Init()
{
	return true;
}

void CPlayerIdleState::Start()
{
}

CState* CPlayerIdleState::Update(float deltaTime)
{
	mRecoveryTimer += deltaTime;
	return CState::Update(deltaTime);
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
	mInven = static_cast<CD2Inventory*>(mOwner->GetGameObject()->FindObjectComponentFromType<CD2Inventory>());
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Push, this, &CPlayerIdleState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Down, this, &CPlayerIdleState::OnDownMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseLCtrl", eKeyState::KeyState_Push, this, &CPlayerIdleState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseR", eKeyState::KeyState_Down, this, &CPlayerIdleState::OnClickMouseR);
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Push, this, &CPlayerIdleState::OnCtrlDown);
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Up, this, &CPlayerIdleState::OnCtrlUp);

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Idle" + std::to_string((int)spriteDir));
}

CState* CPlayerIdleState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	// 플레이어 Hp, Mp 상시 회복
	if (mRecoveryTimer >= 3.f)
	{
		mRecoveryTimer = 0.f;
		state->GetCharInfo()->SetHp(state->GetCharInfo()->GetMaxHp() / 10.f);
		state->GetCharInfo()->SetMp(state->GetCharInfo()->GetMaxMp() / 10.f);
	}

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
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	// 클릭 가능한 오브젝트에 마우스가 올라가 있는 경우, 클릭처리 한다.
	if (state->GetClickableObject())
	{
		state->GetClickableObject()->OnClick();
	}
	// 이외의 경우, 스킬 시전
	else
	{
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
}

void CPlayerIdleState::OnDownMouseL(float deltaTime)
{
	// 들고 있는 아이템이 있는 경우, 아이템 드랍하고, Push시 이동하지 않도록 한다.
	if (mInven->GetGrabItem())
	{
		mbGrabItemExist = true;

		if (mInven->DropItem())
		{
			CD2UIManager::GetInst()->GetInventoryWindow()->SetItemSlotTintEnable(false);
			CEngine::GetInst()->SetMouseState(eMouseState::Normal);
			return;
		}
	}
	else
	{
		mbGrabItemExist = false;
	}
}

void CPlayerIdleState::OnPushMouseL(float deltaTime)
{
	if (mbGrabItemExist)
	{
		return;
	}

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
