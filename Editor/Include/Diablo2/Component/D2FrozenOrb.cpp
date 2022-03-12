#include "D2FrozenOrb.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"

CD2FrozenOrb::CD2FrozenOrb() :
	mRotationSpeed(60.f),
	mFireInterval(0.05f),
	mFireTimer(0.f),
	mExplodeTimer(0.f),
	mIceBolt(nullptr)
{
	SetTypeID<CD2FrozenOrb>();
}

CD2FrozenOrb::CD2FrozenOrb(const CD2FrozenOrb& com)	:
	CD2SkillObject(com)
{
	mRotation = com.mRotation->Clone();
	mIceBolt = com.mIceBolt->Clone();
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
		mRotation->AddWorldRot(0.f, 0.f, 60.f);

		mFireTimer += deltaTime;
		mExplodeTimer += deltaTime;

		if (mExplodeTimer >= mInfo.LifeTime)
		{
			explode();
		}

		if (mFireTimer >= mFireInterval)
		{
			Vector3 axis = mRotation->GetWorldAxis(eAXIS::AXIS_X);
			axis.Normalize();
			fireIceBolt(axis);
		}
	}
}

void CD2FrozenOrb::Start()
{
	CSpriteComponent* com = mObject->FindSceneComponentFromType<CSpriteComponent>();
	com->SetCurrentAnimation("FrozenOrb0");

	mRotation = mObject->FindSceneComponentFromType<CSceneComponent>();
	std::string outName;
	mIceBolt = mObject->GetScene()->LoadGameObject(outName, "IceBolt.gobj");
	mIceBolt->Enable(false);
	mIceBolt->SetWorldPos(mObject->GetWorldPos());
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
	CGameObject* clone = mIceBolt->Clone();
	clone->Enable(true);
	clone->SetWorldPos(mObject->GetWorldPos());

	CD2SkillObject* script = static_cast<CD2SkillObject*>(clone->FindComponent("Script"));
	script->SetDir(Vector2(dir.x, dir.y));
	clone->Start();
}

void CD2FrozenOrb::explode()
{
	CSpriteComponent* com = mObject->FindSceneComponentFromType<CSpriteComponent>();
	com->SetCurrentAnimation("FrozenOrbExplode0");
	com->SetEndCallBack("FrozenOrbExplode0", this, &CD2FrozenOrb::onExplodeEnd);
}

void CD2FrozenOrb::onExplodeEnd()
{
	Vector3 circlePos[31] = {};
	circlePos[0] = Vector3(0.5f, 0.f, 0.f);

	for (int i = 1; i < 31; ++i)
	{
		circlePos[i].x = cosf(DegToRad(12.f * i)) * 0.5f;
		circlePos[i].y = sinf(DegToRad(12.f * i)) * 0.5f;

		fireIceBolt(circlePos[i]);
	}
}
