#include "D2PlayerCollider.h"
#include "Component/ColliderBox2D.h"
#include "Collision/Collision.h"
#include "GameObject/GameObject.h"
#include "D2StateComponent.h"

CD2PlayerCollider::CD2PlayerCollider()
{
	SetTypeID<CD2PlayerCollider>();
}

CD2PlayerCollider::CD2PlayerCollider(const CD2PlayerCollider& com)	:
	CObjectComponent(com)
{
}

CD2PlayerCollider::~CD2PlayerCollider()
{
	mCollider->DeleteCollisionCallBack((void*)this ,eCollisionState::Enter);
	mCollider->DeleteCollisionCallBack((void*)this, eCollisionState::Exit);
}

bool CD2PlayerCollider::Init()
{
	return true;
}

void CD2PlayerCollider::Start()
{
	CObjectComponent::Start();

	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mState = mObject->FindObjectComponentFromType<CD2StateComponent>();

	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2PlayerCollider::OnCollideEnter);
	mCollider->AddCollisionCallBack(eCollisionState::Exit, this, &CD2PlayerCollider::OnCollideExit);
}

void CD2PlayerCollider::Update(float deltaTime)
{
}

void CD2PlayerCollider::PostUpdate(float deltaTime)
{
}

void CD2PlayerCollider::PrevRender()
{
}

void CD2PlayerCollider::Render()
{
}

void CD2PlayerCollider::PostRender()
{
}

CD2PlayerCollider* CD2PlayerCollider::Clone()
{
	return new CD2PlayerCollider(*this);
}

void CD2PlayerCollider::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();

	switch (profile->Channel)
	{
	case eCollisionChannel::Object:
	case eCollisionChannel::Monster:
	{
 //		Box2DInfo srcInfo = static_cast<CColliderBox2D*>(result.Src)->GetInfo();
 //		Box2DInfo destInfo = static_cast<CColliderBox2D*>(result.Dest)->GetInfo();
 //		Box2DInfo overLap = CCollision::GetOverlapAreaAABB(srcInfo, destInfo);
 //
 //		Vector3 colDir = result.Dest->GetWorldPos() - result.Src->GetWorldPos();
 //
 //		Vector2 moveAmount;
 //
 //		if (overLap.Length.x > overLap.Length.y)
 //		{
 //			if (colDir.y > 0)
 //			{
 //				moveAmount.y = -overLap.Length.y * 2.f;
 //			}
 //			else
 //			{
 //				moveAmount.y = overLap.Length.y * 2.f;
 //			}
 //		}
 //		else
 //		{
 //			if (colDir.x > 0)
 //			{
 //				moveAmount.x = -overLap.Length.x * 2.f;
 //			}
 //			else
 //			{
 //				moveAmount.x = overLap.Length.x * 2.f;
 //			}
 //		}
 //
 //		mObject->AddWorldPos(moveAmount.x, moveAmount.y, 0.f);
 		break;
	}
	case eCollisionChannel::MonsterAttack:
	{
		// 局聪皋捞记 贸府
		mState->OnCollideEnter(result);
		break;
	}
	}
}

void CD2PlayerCollider::OnCollideExit(const CollisionResult& result)
{
}
