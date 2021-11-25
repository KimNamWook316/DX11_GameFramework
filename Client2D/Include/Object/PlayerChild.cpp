#include "PlayerChild.h"
#include "Scene/Scene.h"

CPlayerChild::CPlayerChild()	:
	mbIsCaptured(false)
{
}

CPlayerChild::CPlayerChild(const CPlayerChild& obj)	:
	CGameObject(obj)
{
}

CPlayerChild::~CPlayerChild()
{
}

bool CPlayerChild::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("thisSprite");

	SetRootSceneComponent(mSprite);

	mSprite->SetWorldScale(50.f, 50.f, 1.f);
	mSprite->SetWorldPos(800.f, 320.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayerChild::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	if (!mbIsCaptured)
	{
		Vector3 playerPos = mScene->GetPlayerObj()->GetWorldPos();

		Vector3 lookDir = playerPos - GetWorldPos();
		lookDir.Normalize();

		mLookAngle = lookDir.Angle(Vector3::Axis[eAXIS::AXIS_Y]);

		if (playerPos.x > GetWorldPos().x)
		{
			mLookAngle *= -1.f;
		}

		SetWorldRotZ(mLookAngle);

		if (GetWorldPos().Distance(playerPos) <= 200.f)
		{
			mbIsCaptured = true;
			mScene->GetPlayerObj()->AddChildGameObject(this);
			SetRelativePos(lookDir * -200.f);
			return;
		}
	}
	else
	{
		AddRelativeRotZ(180.f * deltaTime);
	}
}

void CPlayerChild::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CGameObject* CPlayerChild::Clone()
{
	return new CPlayerChild(*this);
}
