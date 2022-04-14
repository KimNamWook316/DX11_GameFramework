#include "D2ClickableObject.h"
#include "Scene/Scene.h"
#include "Component/ColliderBox2D.h"
#include "GameObject/GameObject.h"
#include "D2StateComponent.h"
#include "D2ClickableUIComponent.h"
#include "D2UIManager.h"

CD2ClickableObject::CD2ClickableObject()	:
	mCollider(nullptr),
	mPlayerState(nullptr),
	mUI(nullptr)
{
	SetTypeID<CD2ClickableObject>();
}

CD2ClickableObject::CD2ClickableObject(const CD2ClickableObject& com)	:
	CObjectComponent(com)
{
	mPlayerState = com.mPlayerState;
}

CD2ClickableObject::~CD2ClickableObject()
{
}

bool CD2ClickableObject::Init()
{
	return true;
}

void CD2ClickableObject::Start()
{
	CObjectComponent::Start();

	if (!mCollider)
	{
		mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	}

	mCollider->AddCollisionMouseCallBack(eCollisionState::Enter, this, &CD2ClickableObject::onMouseCollideEnter);
	mCollider->AddCollisionMouseCallBack(eCollisionState::Exit, this, &CD2ClickableObject::onMouseCollideExit);
	
	mUI = mObject->FindSceneComponentFromType<CD2ClickableUIComponent>();
}

void CD2ClickableObject::Update(float deltaTime)
{
}

void CD2ClickableObject::PostUpdate(float deltaTime)
{
}

void CD2ClickableObject::PrevRender()
{
}

void CD2ClickableObject::Render()
{
}

void CD2ClickableObject::PostRender()
{
}

void CD2ClickableObject::onMouseCollideEnter(const CollisionResult& result)
{
	if (!mPlayerState)
	{
		mPlayerState =mObject->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2StateComponent>();
	}

	// ���� �Ÿ� �̻� ����� ���¿��� ���콺 Hover�Ǹ� ���� ������Ʈ �÷��̾� State�� ���
	CGameObject* playerObj = mObject->GetScene()->GetPlayerObj();
	float dist = playerObj->GetWorldPos().Distance(mObject->GetWorldPos());

	if (dist <= 150.f)
	{
		mPlayerState->SetClickableObject(this);
	}

	// ���콺�� �浹�ϸ� UI Ȱ��ȭ
	mUI->RenderEnable(true);
}

void CD2ClickableObject::onMouseCollideExit(const CollisionResult& result)
{
	mPlayerState->SetClickableObject(nullptr);

	if (!CD2UIManager::GetInst()->IsTABDown())
	{
		// UI ��Ȱ��ȭ
		mUI->RenderEnable(false);
	}
}
