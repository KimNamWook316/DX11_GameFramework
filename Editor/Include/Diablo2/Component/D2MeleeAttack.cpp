#include "D2MeleeAttack.h"
#include "D2CharacterInfoComponent.h"
#include "GameObject/GameObject.h"

CD2MeleeAttack::CD2MeleeAttack()
{
	SetTypeID<CD2MeleeAttack>();
}

CD2MeleeAttack::CD2MeleeAttack(const CD2MeleeAttack& com)	:
	CD2SkillObject(com)
{

}

CD2MeleeAttack::~CD2MeleeAttack()
{
}

void CD2MeleeAttack::OnAttackAnimEnd()
{
	if (mTargetObj)
	{
		mTargetObj->FindObjectComponentFromType<CD2CharacterInfoComponent>()->SetHp(-mInfo.Damage);
	}
}

CObjectComponent* CD2MeleeAttack::Clone()
{
	return new CD2MeleeAttack(*this);
}

void CD2MeleeAttack::OnCollideEnter(const CollisionResult& result)
{
	// Collider ¾øÀ½
}
