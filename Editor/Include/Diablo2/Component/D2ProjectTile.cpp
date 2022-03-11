#include "D2ProjectTile.h"
#include "GameObject/GameObject.h"
#include "Component/SceneComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/SpriteComponent.h"
#include "D2CharacterInfoComponent.h"
#include "D2DataManager.h"
#include "../D2Util.h"

CD2Projectile::CD2Projectile()
{
	SetTypeID<CD2Projectile>();
}

CD2Projectile::CD2Projectile(const CD2Projectile& com)	:
	CD2SkillObject(com)
{
	mCollider = com.mObject->FindSceneComponentFromType<CColliderCircle>()->Clone();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Projectile::OnCollideEnter);
}

CD2Projectile::~CD2Projectile()
{
}

bool CD2Projectile::Init()
{
	CD2SkillObject::Init();
	mCollider = mObject->FindSceneComponentFromType<CColliderCircle>();

	if (!mCollider)
	{
		assert(false);
		return false;
	}

	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Projectile::OnCollideEnter);


	return true;
}

void CD2Projectile::Start()
{
	CD2SkillObject::Start();
	if (!mCollider)
	{
		mCollider = mObject->FindSceneComponentFromType<CColliderCircle>();

		if (!mCollider)
		{
			assert(false);
			return;
		}

		mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Projectile::OnCollideEnter);
	}

	CSpriteComponent* sprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	
	if (!mInfo.Name.empty())
	{
		sprite->SetCurrentAnimation(mInfo.Name + std::to_string((int)CD2Util::GetSpriteDir(mInfo.Dir)));
	}
}

CD2Projectile* CD2Projectile::Clone()
{
	return new CD2Projectile(*this);
}

void CD2Projectile::SetDir(const Vector2& dir)
{
	CD2SkillObject::SetDir(dir);
	mObject->FindSceneComponentFromType<CSpriteComponent>()->SetCurrentAnimation(mInfo.Name + std::to_string((int)CD2Util::GetSpriteDir(mInfo.Dir)));
}

void CD2Projectile::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
		com->SetCC(mInfo.eElementType);
	}

	mObject->Destroy();
}
