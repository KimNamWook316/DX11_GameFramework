#include "D2AndarielMoveState.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "D2EnemyHitState.h"
#include "D2AndarielMeleeAttackState.h"
#include "D2AndarielDieState.h"
#include "D2AndarielProjectileState.h"
#include "D2AndarielSprayState.h"
#include "Component/ColliderComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CD2AndarielMoveState::CD2AndarielMoveState()	:
	mTimer(1.f),
	mDist(0.f)
{
}

CD2AndarielMoveState::CD2AndarielMoveState(const CD2AndarielMoveState& state)	:
	CD2State(state)
{
}

CD2AndarielMoveState::~CD2AndarielMoveState()
{
}

bool CD2AndarielMoveState::Init()
{
	return true;
}

void CD2AndarielMoveState::Start()
{
}

CState* CD2AndarielMoveState::Update(float deltaTime)
{
	mTimer += deltaTime;

	mDist = mOwner->GetScene()->GetPlayerObj()->GetWorldPos().Distance(mOwner->GetGameObject()->GetWorldPos());

	if (mDist >= 300.f)
	{
		mAttackTimer += deltaTime;
	}

	return CState::Update(deltaTime);
}

void CD2AndarielMoveState::PostUpdate(float deltaTime)
{
}

CD2AndarielMoveState* CD2AndarielMoveState::Clone()
{
	return new CD2AndarielMoveState(*this);
}

void CD2AndarielMoveState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());

	mOwner->GetNavAgent()->SetFrameCallBack(this, &CD2AndarielMoveState::OnMove);
	mOwner->GetNavAgent()->SetEndCallBack(this, &CD2AndarielMoveState::OnMoveEnd);

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	std::string name = mOwner->GetGameObject()->GetName();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation(name + "Move" + std::to_string((int)spriteDir / 2));
}

CState* CD2AndarielMoveState::StateFunction()
{
	if (!mbAttack)
	{
		if (mAttackTimer >= 2.f)
		{
			mAttackTimer = 0.f;

			bool random = (bool)((int)(rand() % 2));
			if (random)
			{
				return (CState*)(new CD2AndarielProjectileState);
			}
			else
			{
				return (CState*)(new CD2AndarielSprayState);
			}
		}
	}
	if (mbDie)
	{
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(0.f);
		return (CState*)(new CD2AndarielDieState);
	}
	else if (mTimer >= 0.5f)
	{
		mTimer = 0.f;
		CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
		state->GetCharInfo()->SetSpeed(state->GetCharInfo()->GetMaxSpeed());
		static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->Move();
	}
	else if (mbAttack)
	{
		return (CState*)(new CD2AndarielMeleeAttackState);
	}
	return nullptr;
}

void CD2AndarielMoveState::ExitStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	mOwner->GetNavAgent()->DeleteEndCallBack();
	mOwner->GetNavAgent()->DeleteFrameCallBack();
}

void CD2AndarielMoveState::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();
	// 이동 상태에서 충돌하는 경우 : 플레이어 조우, 피격, 다른 몬스터와 충돌
	// 다른 몬스터와 충돌하는 경우, Idle 상태로 돌아가고, 이동은 잠깐 중단한다.
	switch (profile->Channel)
	{
	case eCollisionChannel::Player:
		mbAttack = true;
		break;
	case eCollisionChannel::Monster:
	{
		Vector2 myDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetDir();
		Vector2 destDir = static_cast<CD2StateComponent*>(result.Dest->GetGameObject()->FindObjectComponentFromType<CD2StateComponent>())
			->GetCharInfo()->GetDir();

		// 진행 방향 상 앞을 가로막힌 경우에 다시 길을 찾는다.
		if ((myDir.x * destDir.x < 0) && (myDir.y * destDir.y < 0))
		{
			static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->MoveBlockedFront();
		}
		break;
	}
	}
}

void CD2AndarielMoveState::OnCollideExit(const CollisionResult& result)
{
}

void CD2AndarielMoveState::ResetState()
{
	CD2State::ResetState();

	mbAttack = false;
}

void CD2AndarielMoveState::OnMove()
{
	Vector3 direction = mOwner->GetNavAgent()->GetPathListFront() - mOwner->GetRootComponent()->GetWorldPos();
	direction.Normalize();
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	state->GetCharInfo()->SetDir(Vector2(direction.x, direction.y));

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	if (mPrevDir != spriteDir)
	{
		mPrevDir = spriteDir;
		std::string name = mOwner->GetGameObject()->GetName();
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->ChangeAnimation(name + "Move" + std::to_string((int)mPrevDir / 2));
	}
}

void CD2AndarielMoveState::OnMoveEnd()
{
	// 1타일 내 플레이어가 있으면 공격상태로 전환
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
		mbAttack = true;
	}
}
