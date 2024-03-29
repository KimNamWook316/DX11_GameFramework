#include "D2ProjectTile.h"
#include "GameObject/GameObject.h"
#include "Component/SceneComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/SpriteComponent.h"
#include "D2CharacterInfoComponent.h"
#include "D2DataManager.h"
#include "D2ObjectPool.h"
#include "Resource/Sound/Sound.h"
#include "../D2Util.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager.h"

CD2Projectile::CD2Projectile()	:
	mDist(500.f)
{
	SetTypeID<CD2Projectile>();
}

CD2Projectile::CD2Projectile(const CD2Projectile& com)	:
	CD2SkillObject(com)
{
	mDist = com.mDist;
}

CD2Projectile::~CD2Projectile()
{
	mCollider->DeleteCollisionCallBack(this, eCollisionState::Enter);
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

void CD2Projectile::Update(float deltaTime)
{
	CD2SkillObject::Update(deltaTime);

	if (mSkillOwner == mObject->GetScene()->GetPlayerObj())
	{
		if (!mScene->GetNavigationManager()->IsReachableTile(mObject->GetWorldPos()))
		{
			mObject->Destroy();
		}
	}

	float dist = mStartPos.Distance(mObject->GetWorldPos());

	if (dist >= mDist)
	{
		mObject->Destroy();
	}
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

	switch (mInfo.eElementType)
	{
	case eD2ElementType::Fire:
		CD2ObjectPool::GetInst()->ActiveEffect("FireExplode" , mObject->GetWorldPos());
		break;
	case eD2ElementType::Ice:
		CD2ObjectPool::GetInst()->ActiveEffect("IceExplode" , mObject->GetWorldPos());
		break;
	default:
		break;
	}

	mObject->Destroy();
}
