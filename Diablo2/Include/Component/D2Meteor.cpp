#include "D2Meteor.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Component/ColliderCircle.h"
#include "D2CharacterInfoComponent.h"
#include "D2ObjectPool.h"
#include <time.h>

CD2Meteor::CD2Meteor()  :
	mbEnd(false)
{
	SetTypeID<CD2Meteor>();
}

CD2Meteor::CD2Meteor(const CD2Meteor& com)  :
    CD2SkillObject(com)
{
	mbEnd = false;
}

CD2Meteor::~CD2Meteor()
{
	mCollider->DeleteCollisionCallBack(this, eCollisionState::Enter);
}

bool CD2Meteor::Init()
{
	CD2SkillObject::Init();
    return true;
}

void CD2Meteor::Update(float deltaTime)
{
	CD2SkillObject::Update(deltaTime);

	if (!mbEditMode)
	{
		float dist = mObject->GetWorldPos().Distance(mTargetPos);

		if (dist <= 5.f)
		{
			mCollider->Enable(true);
			mCollider->SetWorldPos(mObject->GetWorldPos());
			mbEnd = true;
			mObject->GetScene()->GetResource()->SoundPlay("Meteor");

			spreadFire();
		}

		// 충돌 처리하고, 다음 프레임에 이 오브젝트 삭제
		if (mbEnd)
		{
			mObject->Destroy();
		}
	}
}

void CD2Meteor::Start()
{
	CD2SkillObject::Start();

	mCollider = mObject->FindSceneComponentFromType<CColliderCircle>();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Meteor::OnCollideEnter);
	mCollider->Enable(false);

	CSpriteComponent* sprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	sprite->SetCurrentAnimation("Meteor0");
}

CD2Meteor* CD2Meteor::Clone()
{
	return new CD2Meteor(*this);
}

void CD2Meteor::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
		com->SetCC(mInfo.eElementType);
	}
}

void CD2Meteor::spreadFire()
{
	srand(unsigned(time(NULL)));
	int radiusMin = 10;
	int radiusMax = 51;

	// 랜덤 범위에 불 오브젝트 생성
	for (int i = 0; i < 10; ++i)
	{
		Vector3 randomDir;

		randomDir.x = (-10.f + (float)(rand() % 21)) / 10.f;
		randomDir.y = (-10.f + (float)(rand() % 21)) / 10.f;
		randomDir.Normalize();

		float randomRad = 0.f;
		randomRad = radiusMin + (rand() % (radiusMax - radiusMin));

		Vector3 randomPos = mObject->GetWorldPos() + randomDir * randomRad;

		CGameObject* fireObj = CD2ObjectPool::GetInst()->CloneSkillObj("MeteorFire");

		if (!fireObj)
		{
			assert(false);
			return;
		}

		fireObj->Enable(true);
		fireObj->SetWorldPos(randomPos);
		fireObj->Start();

		CD2ObjectPool::GetInst()->ActiveEffect("FireExplode", randomPos);
	}
}
