#include "D2MeteorFire.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"
#include "D2DataManager.h"

CD2MeteorFire::CD2MeteorFire()	:
	mTimer(0.f),
	mbEnd(false)
{
	SetTypeID<CD2MeteorFire>();
}

CD2MeteorFire::CD2MeteorFire(const CD2MeteorFire& com)	:
	CD2SkillObject(com)
{
	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2MeteorFire::OnCollideEnter);
	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
}

CD2MeteorFire::~CD2MeteorFire()
{
}

bool CD2MeteorFire::Init()
{
	CD2SkillObject::Init();
	return true;
}

void CD2MeteorFire::Update(float deltaTime)
{
	CD2SkillObject::Update(deltaTime);

	if (!mbEditMode)
	{
		if (!mbEnd)
		{
			mTimer += deltaTime;

			if (mTimer >= mInfo.LifeTime)
			{
				mSprite->ChangeAnimation("SmallFireEnd0");
				mSprite->SetEndCallBack("SmallFireEnd0", this, &CD2MeteorFire::OnEndAnimEnd);
				mbEnd = true;
			}
		}
	}
}

void CD2MeteorFire::Start()
{
	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();

	if (!mCollider)
	{
		assert(false);
		return;
	}

	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2MeteorFire::OnCollideEnter);

	mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	mSprite->ChangeAnimation("SmallFireStart0");
	mSprite->SetEndCallBack("SmallFireStart0", this, &CD2MeteorFire::OnStartAnimEnd);

	if (!mbEditMode)
	{
		D2SkillInfo* info = CD2DataManager::GetInst()->FindSkillInfo("MeteorFire");
		mInfo = *info;
	}
}

CD2MeteorFire* CD2MeteorFire::Clone()
{
	return new CD2MeteorFire(*this);
}

void CD2MeteorFire::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
		com->SetCC(mInfo.eElementType);
	}
}

void CD2MeteorFire::OnStartAnimEnd()
{
	mSprite->ChangeAnimation("SmallFire0");
}

void CD2MeteorFire::OnEndAnimEnd()
{
	mObject->Destroy();
}
