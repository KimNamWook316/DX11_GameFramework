#include "D2Blaze.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"
#include "Scene/NavigationManager.h"
#include "Scene/Scene.h"

CD2Blaze::CD2Blaze()	:
	mTimer(0.f),
	mbEnd(false),
	mbRegen(false),
	mParent(nullptr),
	mChild(nullptr),
	mGlobalLifeTimer(0.f),
	mTileIdx(-1)
{
	SetTypeID<CD2Blaze>();
}

CD2Blaze::CD2Blaze(const CD2Blaze& com)	:
	CD2SkillObject(com)
{
	mTileIdx = -1;
	mTimer = 0.f;
	mbEnd = false;
	mbRegen = false;
	mGlobalLifeTimer = com.mGlobalLifeTimer;
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
		mGlobalLifeTimer += deltaTime;
		
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

		if (!mbRegen && mGlobalLifeTimer <= mInfo.LifeTime)
		{
			int playerIdx = mScene->GetNavigationManager()->GetIndex(mSkillOwner->GetWorldPos());

			if (playerIdx != mTileIdx)
			{
				CGameObject* obj = mObject->Clone();
				CD2Blaze* com = obj->FindObjectComponentFromType<CD2Blaze>();
				obj->Start();
				obj->SetWorldPos(mSkillOwner->GetWorldPos());
				mChild = com;
				com->SetParent(this);
				com->SetTileIdx(playerIdx);
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

void CD2Blaze::SetSkillOwner(CGameObject* obj)
{
	CD2SkillObject::SetSkillOwner(obj);
	mTileIdx = mScene->GetNavigationManager()->GetIndex(obj->GetWorldPos());
	mObject->SetWorldPos(obj->GetWorldPos());
}
