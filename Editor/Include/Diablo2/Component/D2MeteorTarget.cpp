#include "D2MeteorTarget.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "D2PlayerSkillComponent.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "D2ObjectPool.h"

CD2MeteorTarget::CD2MeteorTarget()	:
	mTimer(0.f),
	mbFirstFrame(true)
{
	SetTypeID<CD2MeteorTarget>();
}

CD2MeteorTarget::CD2MeteorTarget(const CD2MeteorTarget& com)	:
	CD2SkillObject(com)
{
	mTimer = 0.f;
	mbFirstFrame = true;
}

CD2MeteorTarget::~CD2MeteorTarget()
{
}

bool CD2MeteorTarget::Init()
{
	CD2SkillObject::Init();
	return true;
}

void CD2MeteorTarget::Update(float deltaTime)
{
	if (mbFirstFrame)
	{
		mObject->SetWorldPos(mTargetPos);
		mbFirstFrame = false;
	}

	if (!mbEditMode)
	{
		mTimer += deltaTime;

		if (mTimer >= mInfo.LifeTime)
		{
			CGameObject* obj = CD2ObjectPool::GetInst()->CloneSkillObj("Meteor");

			if (!obj)
			{
				assert(false);
				return;
			}
			obj->Enable(true);
			obj->Start();

			// �Ʒ����� ���������� 30�� Ʋ���� ����
 //			Matrix matRot;
 //			matRot.Rotation(0.f, 0.f, 30.f);
			Vector3 dir = Vector3(0.f, -1.f, 0.f);
 //			dir.TransformCoord(matRot);
			dir.Normalize();

			CD2SkillObject* script = static_cast<CD2SkillObject*>(obj->FindComponent("Script"));

			// ����� ���׿� ��ü �浹���� ���� 
			script->SetSkillOwner(mSkillOwner);
			script->SetTargetPos(mObject->GetWorldPos());
			script->SetDir(Vector2(dir.x, dir.y));

			// 1�� �� Ÿ�� ������ �����ϴ� ��ġ�� �̵�
			Vector3 reverseDir = Vector3(dir.x, -dir.y, 0.f);
			obj->SetWorldPos(mObject->GetWorldPos() + (reverseDir * 800.f));

			mObject->Destroy();
		}
	}
}

void CD2MeteorTarget::Start()
{
	CSpriteComponent* sprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	sprite->SetCurrentAnimation("MeteorTarget0");
}

CD2MeteorTarget* CD2MeteorTarget::Clone()
{
	return new CD2MeteorTarget(*this);
}

void CD2MeteorTarget::OnCollideEnter(const CollisionResult& result)
{
}
