#include "D2AndarielMeleeAttackState.h"
#include "D2EnemyHitState.h"
#include "D2AndarielDieState.h"
#include "D2AndarielSprayState.h"
#include "Component/ColliderComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "../Component/D2EnemyMeleeAttack.h"
#include "../Component/D2EnemySkillComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CD2AndarielMeleeAttackState::CD2AndarielMeleeAttackState()
{
}

CD2AndarielMeleeAttackState::CD2AndarielMeleeAttackState(const CD2AndarielMeleeAttackState& state)	:
	CD2State(state)
{
}

CD2AndarielMeleeAttackState::~CD2AndarielMeleeAttackState()
{
}

bool CD2AndarielMeleeAttackState::Init()
{
	return true;
}

void CD2AndarielMeleeAttackState::Start()
{

}

void CD2AndarielMeleeAttackState::PostUpdate(float deltaTime)
{
}

CD2AndarielMeleeAttackState* CD2AndarielMeleeAttackState::Clone()
{
	return new CD2AndarielMeleeAttackState(*this);
}

void CD2AndarielMeleeAttackState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	state->GetCharInfo()->SetSpeed(0.f);
	Vector3 dir = mOwner->GetScene()->GetPlayerObj()->GetWorldPos() - mOwner->GetGameObject()->GetWorldPos();
	state->GetCharInfo()->SetDir(Vector2(dir.x, dir.y));
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "MeleeAttack" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetLoop(animName, true);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2AndarielMeleeAttackState::OnAnimEnd);
	mOwner->GetGameObject()->AddWorldScale(70.f, 70.f, 0.f);
	mOwner->GetGameObject()->SetPivot(0.5f, 0.2f, 0.f);
}

CState* CD2AndarielMeleeAttackState::StateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	if (mbDie)
	{
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2AndarielDieState);
	}
	else if (mbAnimEnd && !mbKeepAttack)
	{
		mbEnd = true;
		return nullptr;
	}
	else if (mbHit)
	{
		mbEnd = true;
		return (CState*)(new CD2EnemyHitState);
	}
	else if (mbSpray)
	{
		static_cast<CD2EnemyNavAgentComponent*>(state->GetNavAgent())->CancleMove();
		return (CState*)(new CD2AndarielSprayState);
	}
	return nullptr;
}

void CD2AndarielMeleeAttackState::ExitStateFunction()
{
	mOwner->GetGameObject()->AddWorldScale(-70.f, -70.f, 0.f);
	mOwner->GetGameObject()->SetPivot(0.5f, 0.f, 0.f);
}

void CD2AndarielMeleeAttackState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// 공격 상태에서 피격당할 경우, 이 상태에서 바로 벗어난다.
	switch (profile->Channel)
	{
	case eCollisionChannel::PlayerAttack:
		mbHit = true;
		break;
	}
}

void CD2AndarielMeleeAttackState::OnCollideExit(const CollisionResult& result)
{
}

void CD2AndarielMeleeAttackState::ResetState()
{
	CD2State::ResetState();

	mbHit = false;
	mbKeepAttack = false;
	mbSpray = false;
	mbAnimEnd = false;
}

void CD2AndarielMeleeAttackState::OnAnimEnd()
{
	mbAnimEnd = true;

	// 플레이어가 1타일 범위 내에 있으면 계속 공격상태 유지
	CScene* scene = mOwner->GetScene();
	CTileMapComponent* tileMap = scene->GetNavigationManager()->GetNavData();

	int playerTileIdxX = tileMap->GetPathFindTileIndexX(scene->GetPlayerObj()->GetWorldPos());
	int playerTileIdxY = tileMap->GetPathFindTileIndexY(scene->GetPlayerObj()->GetWorldPos());

	int myTileIdxX = tileMap->GetPathFindTileIndexX(mOwner->GetGameObject()->GetWorldPos());
	int myTileIdxY = tileMap->GetPathFindTileIndexY(mOwner->GetGameObject()->GetWorldPos());

	int tileDistX = abs(playerTileIdxX - myTileIdxX);
	int tileDistY = abs(playerTileIdxY - myTileIdxY);

	if (tileDistX <= 1 && tileDistY <= 1)
	{
		mbKeepAttack = true;

		// 스프라이트 방향 조정
		Vector3 playerPos = scene->GetPlayerObj()->GetWorldPos();
		Vector3 myPos = mOwner->GetGameObject()->GetWorldPos();
		Vector3 dir = playerPos - myPos;
		static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->SetDir(Vector2(dir.x, dir.y));

		// 랜덤으로 Spray 공격
		bool random = (bool)((int)(rand() % 2));

		if (random)
		{
			mbSpray = true;
		}
		else
		{
			mAttackObj = static_cast<CD2StateComponent*>(mOwner)->GetEnemySkill()->DoSkill("EnemyMeleeAttack", playerPos, playerPos, Vector2(dir.x, dir.y));

			eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
			std::string animName = mOwner->GetGameObject()->GetName() + "MeleeAttack" + std::to_string((int)spriteDir / 2);
			static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
			static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetLoop(animName, true);
			static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2AndarielMeleeAttackState::OnAnimEnd);
		}

	}
	else
	{
		mbKeepAttack = false;
		mbEnd = true;
	}
}
