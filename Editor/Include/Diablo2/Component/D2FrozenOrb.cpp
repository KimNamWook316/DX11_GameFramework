#include "D2FrozenOrb.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "D2ObjectPool.h"

CD2FrozenOrb::CD2FrozenOrb() :
	mFireInterval(0.05f),
	mFireTimer(0.05f),
	mExplodeTimer(0.f),
	mbExplode(false)
{
	SetTypeID<CD2FrozenOrb>();
}

CD2FrozenOrb::CD2FrozenOrb(const CD2FrozenOrb& com)	:
	CD2SkillObject(com)
{
	mFireInterval = 0.05f;
	mFireTimer = 0.05;
	mExplodeTimer = 0.f;
	mbExplode = false;
}

CD2FrozenOrb::~CD2FrozenOrb()
{
}

bool CD2FrozenOrb::Init()
{
	CD2SkillObject::Init();
	return true;
}

void CD2FrozenOrb::Update(float deltaTime)
{
	CD2SkillObject::Update(deltaTime);

	if (!mbEditMode)
	{
		if (!mbExplode)
		{
			mRotation->AddWorldRot(0.f, 0.f, 33.f);

			mFireTimer += deltaTime;
			mExplodeTimer += deltaTime;

			if (mExplodeTimer >= mInfo.LifeTime)
			{
				explode();
				mbExplode = true;
			}

			if (mFireTimer >= mFireInterval)
			{
				Vector3 axis = mRotation->GetWorldAxis(eAXIS::AXIS_X);
				axis.Normalize();
				fireIceBolt(axis);
				mFireTimer = 0.f;
			}
		}
	}
}

void CD2FrozenOrb::Start()
{
	CD2SkillObject::Start();

	CSpriteComponent* com = mObject->FindSceneComponentFromType<CSpriteComponent>();
	com->SetCurrentAnimation("FrozenOrb0");

	mRotation = mObject->FindSceneComponentFromType<CSceneComponent>();
}

CD2FrozenOrb* CD2FrozenOrb::Clone()
{
	return new CD2FrozenOrb(*this);
}

void CD2FrozenOrb::OnCollideEnter(const CollisionResult& result)
{
}

void CD2FrozenOrb::fireIceBolt(const Vector3& dir)
{
	CGameObject* clone = CD2ObjectPool::GetInst()->CloneSkillObj("IceBolt");
	clone->Enable(true);
	clone->SetWorldPos(mObject->GetWorldPos());

	CD2SkillObject* script = static_cast<CD2SkillObject*>(clone->FindComponent("Script"));
	script->SetDir(Vector2(dir.x, dir.y));
	script->SetStartPos(mObject->GetWorldPos());
	clone->Start();
}

void CD2FrozenOrb::explode()
{
	CSpriteComponent* com = mObject->FindSceneComponentFromType<CSpriteComponent>();
	com->ChangeAnimation("FrozenOrbExplode0");
	com->GetAnimationInstance()->Play();
	com->AddNotify("FrozenOrbExplode0", 0, this, &CD2FrozenOrb::onExplode);
	com->SetEndCallBack("FrozenOrbExplode0", this, &CD2FrozenOrb::onExplodeEnd);
	com->SetWorldScale(100.f, 100.f, 1.f);
}

void CD2FrozenOrb::onExplode()
{
	Vector3 circlePos[17] = {};
	circlePos[0] = Vector3(0.5f, 0.f, 0.f);

	for (int i = 1; i < 17; ++i)
	{
		circlePos[i].x = cosf(DegToRad(22.5f * i)) * 0.5f;
		circlePos[i].y = sinf(DegToRad(22.5f * i)) * 0.5f;

		fireIceBolt(circlePos[i]);
	}
}

void CD2FrozenOrb::onExplodeEnd()
{
	mObject->Destroy();
}
