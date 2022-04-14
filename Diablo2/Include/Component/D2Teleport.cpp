#include "D2Teleport.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager.h"
#include "Component/ColliderBox2D.h"

CD2Teleport::CD2Teleport()	:
	mbIsCollide(false),
	mbStart(false)
{
	SetTypeID<CD2Teleport>();
}

CD2Teleport::CD2Teleport(const CD2Teleport& com)	:
	CD2SkillObject(com)
{
	mbIsCollide = false;
	mbStart = false;
}

CD2Teleport::~CD2Teleport()
{
	mCollider->DeleteCollisionCallBack(this, eCollisionState::Enter);
}

bool CD2Teleport::Init()
{
	CD2SkillObject::Init();
	return true;
}

void CD2Teleport::Update(float deltaTime)
{
	if (!mbStart)
	{
		mObject->SetWorldPos(mTargetPos);
		mbStart = true;
	}
	else
	{
		if (mObject->GetScene()->GetNavigationManager()->IsReachableTile(mTargetPos))
		{
			mSkillOwner->SetWorldPos(mTargetPos);
		}
		
		mObject->Destroy();
	}
}

void CD2Teleport::Start()
{
	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2Teleport::OnCollideEnter);
}

CD2Teleport* CD2Teleport::Clone()
{
	return new CD2Teleport(*this);
}

void CD2Teleport::OnCollideEnter(const CollisionResult& result)
{
	mbIsCollide = true;
}
