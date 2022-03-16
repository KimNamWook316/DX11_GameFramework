#include "D2EnemyMeleeAttack.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"
#include "Component/ColliderBox2D.h"

CD2EnemyMeleeAttack::CD2EnemyMeleeAttack()
{
	SetTypeID<CD2EnemyMeleeAttack>();
}

CD2EnemyMeleeAttack::CD2EnemyMeleeAttack(const CD2EnemyMeleeAttack& com)	:
	CD2SkillObject(com)
{
}

CD2EnemyMeleeAttack::~CD2EnemyMeleeAttack()
{
}

bool CD2EnemyMeleeAttack::Init()
{
	CD2SkillObject::Init();
	return true;
}

void CD2EnemyMeleeAttack::Start()
{
	CD2SkillObject::Start();

	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2EnemyMeleeAttack::OnCollideEnter);
	mCollider->Enable(false);

	mRoot->SetPivot(0.5f, 0.5f, 0.f);
}

CD2EnemyMeleeAttack* CD2EnemyMeleeAttack::Clone()
{
	return new CD2EnemyMeleeAttack(*this);
}

void CD2EnemyMeleeAttack::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
		com->SetCC(mInfo.eElementType);
	}

	mObject->Destroy();
}

void CD2EnemyMeleeAttack::OnCollideExit(const CollisionResult& result)
{
}
