#include "PlayerCastingState.h"
#include "Component/SpriteComponent.h"
#include "Collision/Collision.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2NavAgentComponent.h"
#include "Component/ColliderBox2D.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "PlayerHitState.h"
#include "Component/ColliderComponent.h"
#include "../Component/D2ObjectPool.h"
#include "Scene/Scene.h"

CPlayerCastingState::CPlayerCastingState()
{
}

CPlayerCastingState::CPlayerCastingState(const CPlayerCastingState& state)	:
	CD2State(state)
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
	CD2StateComponent* state = static_cast<CD2StateComponent*>(mOwner);

	eD2SpriteDir spriteDir = state->GetCharInfo()->GetSpriteDir();
	CSpriteComponent* com = static_cast<CSpriteComponent*>(mOwner->GetRootComponent());
	com->SetCurrentAnimation("SpecialOne" + std::to_string((int)spriteDir));
	com->SetEndCallBack("SpecialOne" + std::to_string((int)spriteDir), this, &CPlayerCastingState::OnAnimEnd);
	mOriginSpriteScale = com->GetWorldScale();
	com->AddWorldScale(15.f, 16.6f, 0.f);

	eD2ElementType eElementType = (eD2ElementType)state->GetSkill()->GetRSkillElementType();

	Vector3	worldPos = mOwner->GetGameObject()->GetWorldPos();
	worldPos.y -= 30.f;
	CGameObject* effect = nullptr;
	switch (eElementType)
	{
	case eD2ElementType::Fire:
		effect = CD2ObjectPool::GetInst()->ActiveEffect("FireCast", worldPos);
		// Sound
		mOwner->GetScene()->GetResource()->SoundPlay("FireCast");
		break;
	case eD2ElementType::Ice:
		effect = CD2ObjectPool::GetInst()->ActiveEffect("IceCast", worldPos);
		// Sound
		mOwner->GetScene()->GetResource()->SoundPlay("IceCast");
		break;
	case eD2ElementType::Lightning:
		effect = CD2ObjectPool::GetInst()->ActiveEffect("TeleportCast", worldPos);
		// Sound
		mOwner->GetScene()->GetResource()->SoundPlay("Teleport");
		break;
	}
}

CState* CPlayerCastingState::StateFunction()
{
	return nullptr;
}

void CPlayerCastingState::ExitStateFunction()
{
	CSpriteComponent* com = static_cast<CSpriteComponent*>(mOwner->GetRootComponent());
	com->AddWorldScale(- 15.f, - 16.6f, 0.f);
}

void CPlayerCastingState::ResetState()
{
}

void CPlayerCastingState::OnCollideEnter(const CollisionResult& result)
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

void CPlayerCastingState::OnCollideExit(const CollisionResult& result)
{
}

void CPlayerCastingState::OnAnimEnd()
{
	Vector2 mousePos = static_cast<CD2StateComponent*>(mOwner)->GetPrevStateMousePos();
	Vector3 worldPos = mOwner->GetGameObject()->GetWorldPos();
	worldPos.y += 60.f;
	Vector2 dir = mousePos - Vector2(worldPos.x, worldPos.y);
	dir.Normalize();

	Vector3 reservedTarget = static_cast<CD2StateComponent*>(mOwner)->GetReserveSkillTargetPos();

	static_cast<CD2StateComponent*>(mOwner)->GetSkill()->DoRSkill(worldPos, reservedTarget, dir);

	mbEnd = true;
}
