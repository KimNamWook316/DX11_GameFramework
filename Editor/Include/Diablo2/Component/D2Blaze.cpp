#include "D2Blaze.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"

CD2Blaze::CD2Blaze()	:
	mTimer(0.f),
	mbEnd(false),
	mRegenDist(10.f),
	mbRegen(false)
{
	SetTypeID<CD2Blaze>();
}

CD2Blaze::CD2Blaze(const CD2Blaze& com)	:
	CD2SkillObject(com)
{
	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Blaze::OnCollideEnter);
	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
}

CD2Blaze::~CD2Blaze()
{
}

bool CD2Blaze::Init()
{
	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();

	if (!mCollider)
	{
		assert(false);
		return false;
	}

	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Blaze::OnCollideEnter);

	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	mSprite->ChangeAnimation("FireStart0");
	mSprite->SetEndCallBack("FireStart0", this, &CD2Blaze::OnStartAnimEnd);

	return true;
}

void CD2Blaze::Update(float deltaTime)
{
	CD2SkillObject::Update(deltaTime);

	if (!mbEditMode)
	{
		if (!mbEnd)
		{
			mTimer += deltaTime;

			if (mTimer >= mInfo.LifeTime)
			{
				mSprite->ChangeAnimation("FireEnd0");
				mSprite->SetEndCallBack("FireEnd0", this, &CD2Blaze::OnEndAnimEnd);
				mbEnd = true;
			}
		}

		if (!mbRegen)
		{
			float dist = mSkillOwner->GetWorldPos().Distance(mObject->GetWorldPos());
			if (dist >= mRegenDist)
			{
				CGameObject* obj = mObject->Clone();
				obj->SetWorldPos(mSkillOwner->GetWorldPos());
				mbRegen = true;
			}
		}
	}
}

void CD2Blaze::Start()
{
	if (!mCollider)
	{
		mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();

		if (!mCollider)
		{
			assert(false);
			return;
		}

		mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Blaze::OnCollideEnter);
	}

	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	mSprite->ChangeAnimation("FireStart0");
	mSprite->SetEndCallBack("FireStart0", this, &CD2Blaze::OnStartAnimEnd);
}

CD2Blaze* CD2Blaze::Clone()
{
	return new CD2Blaze(*this);
}

void CD2Blaze::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
		com->SetCC(mInfo.eElementType);
	}
}

void CD2Blaze::OnStartAnimEnd()
{
	mSprite->ChangeAnimation("Fire0");
}

void CD2Blaze::OnEndAnimEnd()
{
	mObject->Destroy();
}
