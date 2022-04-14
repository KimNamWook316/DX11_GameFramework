#include "PlayerHitState.h"
#include "Component/SpriteComponent.h"
#include "../Component/D2StateComponent.h"
#include "Scene/Scene.h"

CPlayerHitState::CPlayerHitState()
{
}

CPlayerHitState::CPlayerHitState(const CPlayerHitState& state)	:
	CD2State(state)
{
}

CPlayerHitState::~CPlayerHitState()
{
}

bool CPlayerHitState::Init()
{
	return true;
}

void CPlayerHitState::Start()
{
}

void CPlayerHitState::PostUpdate(float deltaTime)
{
}

CPlayerHitState* CPlayerHitState::Clone()
{
	return new CPlayerHitState(*this);
}

void CPlayerHitState::EnterStateFunction()
{
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	CSpriteComponent* com = static_cast<CSpriteComponent*>(mOwner->GetRootComponent());
	com->SetCurrentAnimation("Hit" + std::to_string((int)spriteDir));
	com->SetEndCallBack("Hit" + std::to_string((int)spriteDir), this, &CPlayerHitState::OnAnimEnd);

	// Sound
	mOwner->GetScene()->GetResource()->SoundPlay("PlayerHit");
}

CState* CPlayerHitState::StateFunction()
{
	return nullptr;
}

void CPlayerHitState::ExitStateFunction()
{
}

void CPlayerHitState::OnAnimEnd()
{
	mbEnd = true;
}

void CPlayerHitState::OnCollideEnter(const CollisionResult& result)
{
}

void CPlayerHitState::OnCollideExit(const CollisionResult& result)
{
}
