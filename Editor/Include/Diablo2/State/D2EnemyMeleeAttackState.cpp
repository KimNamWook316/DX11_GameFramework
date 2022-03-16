#include "D2EnemyMeleeAttackState.h"
#include "D2EnemyHitState.h"
#include "Component/ColliderComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "../Component/D2EnemyMeleeAttack.h"
#include "../Component/D2EnemySkillComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CD2EnemyMeleeAttackState::CD2EnemyMeleeAttackState()
{
}

CD2EnemyMeleeAttackState::CD2EnemyMeleeAttackState(const CD2EnemyMeleeAttackState& state)	:
	CD2State(state)
{
}

CD2EnemyMeleeAttackState::~CD2EnemyMeleeAttackState()
{
}

bool CD2EnemyMeleeAttackState::Init()
{
	return true;
}

void CD2EnemyMeleeAttackState::Start()
{
	Vector3 dir = mOwner->GetScene()->GetPlayerObj()->GetWorldPos() - mOwner->GetGameObject()->GetWorldPos();
	static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->SetDir(Vector2(dir.x, dir.y));
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "MeleeAttack" + std::to_string((int)spriteDir / 2);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetLoop(animName, true);
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2EnemyMeleeAttackState::OnAnimEnd);
}

void CD2EnemyMeleeAttackState::PostUpdate(float deltaTime)
{
}

CD2EnemyMeleeAttackState* CD2EnemyMeleeAttackState::Clone()
{
	return new CD2EnemyMeleeAttackState(*this);
}

void CD2EnemyMeleeAttackState::EnterStateFunction()
{
}

CState* CD2EnemyMeleeAttackState::StateFunction()
{
	if (mbAnimEnd && !mbKeepAttack)
	{
		mbEnd = true;
		return nullptr;
	}
	else if (mbHit)
	{
		mbEnd = true;
		return (CState*)(new CD2EnemyHitState);
	}
	return nullptr;
}

void CD2EnemyMeleeAttackState::ExitStateFunction()
{
}

void CD2EnemyMeleeAttackState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// ���� ���¿��� �ǰݴ��� ���, �� ���¿��� �ٷ� �����.
	switch (profile->Channel)
	{
	case eCollisionChannel::PlayerAttack:
		mbHit = true;
		break;
	}
}

void CD2EnemyMeleeAttackState::OnCollideExit(const CollisionResult& result)
{
}

void CD2EnemyMeleeAttackState::ResetState()
{
	CD2State::ResetState();

	mbHit = false;
	mbKeepAttack = false;
}

void CD2EnemyMeleeAttackState::OnAnimEnd()
{
	mbAnimEnd = true;

	// �÷��̾ 1Ÿ�� ���� ���� ������ ��� ���ݻ��� ����
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

		// ��������Ʈ ���� ����
		Vector3 playerPos = scene->GetPlayerObj()->GetWorldPos();
		Vector3 myPos = mOwner->GetGameObject()->GetWorldPos();
		Vector3 dir = playerPos - myPos;
		static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->SetDir(Vector2(dir.x, dir.y));

		static_cast<CD2StateComponent*>(mOwner)->GetEnemySkill()->DoSkill("EnemyMeleeAttack", playerPos, playerPos, Vector2(dir.x, dir.y));

		eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
		std::string animName = mOwner->GetGameObject()->GetName() + "MeleeAttack" + std::to_string((int)spriteDir / 2);
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(animName);
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetLoop(animName, true);
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack(animName, this, &CD2EnemyMeleeAttackState::OnAnimEnd);
	}
	else
	{
		mbKeepAttack = false;
		mbEnd = true;
	}
}
