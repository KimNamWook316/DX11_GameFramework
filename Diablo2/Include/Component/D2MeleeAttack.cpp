#include "D2MeleeAttack.h"
#include "D2CharacterInfoComponent.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"

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
	mCollider->DeleteCollisionCallBack(this, eCollisionState::Enter);
}

void CD2MeleeAttack::Start()
{
	CD2SkillObject::Start();

	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2MeleeAttack::OnCollideEnter);
	mCollider->Enable(false);

	mRoot->SetPivot(0.5f, 0.5f, 0.f);
}

CObjectComponent* CD2MeleeAttack::Clone()
{
	return new CD2MeleeAttack(*this);
}

void CD2MeleeAttack::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
		com->SetCC(mInfo.eElementType);
	}

	mObject->Destroy();
}
