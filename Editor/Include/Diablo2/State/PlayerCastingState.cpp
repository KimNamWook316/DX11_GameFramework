#include "PlayerCastingState.h"
#include "Component/SpriteComponent.h"
#include "../Component/D2StateComponent.h"
#include "GameObject/GameObject.h"
#include "Input.h"

CPlayerCastingState::CPlayerCastingState()
{
}

CPlayerCastingState::CPlayerCastingState(const CPlayerCastingState& state)	:
	CState(state)
{
}

CPlayerCastingState::~CPlayerCastingState()
{
}

bool CPlayerCastingState::Init()
{
	return true;
}

void CPlayerCastingState::Start()
{
}

void CPlayerCastingState::PostUpdate(float deltaTime)
{
}

CState* CPlayerCastingState::Clone()
{
	return new CPlayerCastingState(*this);
}

void CPlayerCastingState::EnterStateFunction()
{
	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("SpecialOne" + std::to_string((int)spriteDir));

	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetEndCallBack("SpecialOne" + std::to_string((int)spriteDir), this, &CPlayerCastingState::OnAnimEnd);
}

CState* CPlayerCastingState::StateFunction()
{
	return nullptr;
}

void CPlayerCastingState::ExitStateFunction()
{
}

void CPlayerCastingState::ResetState()
{
}

void CPlayerCastingState::OnAnimEnd()
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	Vector3 worldPos = mOwner->GetGameObject()->GetWorldPos();
	Vector2 dir = mousePos - Vector2(worldPos.x, worldPos.y);
	dir.Normalize();

	static_cast<CD2StateComponent*>(mOwner)->GetSkill()->DoRSkill(worldPos, Vector3(mousePos.x, mousePos.y, 0.f), dir);

	mbEnd = true;
}
