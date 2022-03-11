#include "D2Teleport.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager.h"
#include "Component/ColliderBox2D.h"

CD2Teleport::CD2Teleport()	:
	mbIsCollide(false)
{
	SetTypeID<CD2Teleport>();
}

CD2Teleport::CD2Teleport(const CD2Teleport& com)	:
	CD2SkillObject(com)
{
	mbIsCollide = false;
}

CD2Teleport::~CD2Teleport()
{
}

bool CD2Teleport::Init()
{
	CD2SkillObject::Init();
	return true;
}

void CD2Teleport::Update(float deltaTime)
{
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

void CD2Teleport::OnAnimEnd()
{
	int targetIdx = mObject->GetScene()->GetNavigationManager()->GetIndex(mTargetPos);

	if (-1 == targetIdx)
	{
		return;
	}

	// 타겟 위치에 몬스터나 오브젝트가 있다면
	if (mbIsCollide)
	{
		return;
	}

	mSkillOwner->SetWorldPos(mTargetPos);
}
