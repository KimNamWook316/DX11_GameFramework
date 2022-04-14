#include "D2EnemyDeadState.h"
#include "GameObject/GameObject.h"
#include "Component/DissolveComponent.h"
#include "../Component/D2StateComponent.h"
#include "Component/SpriteComponent.h"
#include "../Component/D2EnemyNavAgentComponent.h"
#include "../Component/D2DungeonManager.h"

CD2EnemyDeadState::CD2EnemyDeadState()
{
}

CD2EnemyDeadState::CD2EnemyDeadState(const CD2EnemyDeadState& state)	:
	CD2State(state)
{
}

CD2EnemyDeadState::~CD2EnemyDeadState()
{
}

bool CD2EnemyDeadState::Init()
{
	return true;
}

void CD2EnemyDeadState::Start()
{
}

void CD2EnemyDeadState::PostUpdate(float deltaTime)
{
}

CD2EnemyDeadState* CD2EnemyDeadState::Clone()
{
	return new CD2EnemyDeadState(*this);
}

void CD2EnemyDeadState::EnterStateFunction()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);
	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	std::string animName = mOwner->GetGameObject()->GetName() + "Dead" + std::to_string((int)spriteDir / 2);
	mDissolve = mOwner->GetGameObject()->FindObjectComponentFromType<CDissolveComponent>();
	mDissolve->SetFinishCallBack(this, &CD2EnemyDeadState::OnDissolveEnd);
	mDissolve->SetMaterial(static_cast<CSpriteComponent*>(mOwner->GetGameObject()->GetRootSceneComponent())->GetMaterial());
	mDissolve->StartDissolve();
}

CState* CD2EnemyDeadState::StateFunction()
{
	return nullptr;
}

void CD2EnemyDeadState::ExitStateFunction()
{
}

void CD2EnemyDeadState::OnCollideEnter(const CollisionResult& result)
{
}

void CD2EnemyDeadState::OnCollideExit(const CollisionResult& result)
{
}

void CD2EnemyDeadState::ResetState()
{
	CD2State::ResetState();
}

void CD2EnemyDeadState::OnDissolveEnd()
{
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	// 스폰된 방에 Dead 이벤트 알림
	CD2DungeonManager::GetInst()->OnMonsterDead(state->GetCharInfo()->GetSpawnRoomIdx());

	// 점유하고 있던 타일맵 이전 상태로 되돌리고, 오브젝트 삭제
	static_cast<CD2EnemyNavAgentComponent*>(mOwner->GetNavAgent())->OnDie();
	mOwner->GetGameObject()->Destroy();
}
