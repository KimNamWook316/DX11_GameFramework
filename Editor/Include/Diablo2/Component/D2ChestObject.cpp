#include "D2ChestObject.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/DissolveComponent.h"
#include "D2ItemObjectComponent.h"
#include "D2ClickableUIComponent.h"
#include "../UI/D2ClickableObjectWindow.h"
#include "D2Item.h"
#include "Scene/Scene.h"
#include "D2ItemTable.h"
#include "D2ObjectPool.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager.h"

CD2ChestObject::CD2ChestObject()	:
	mSprite(nullptr),
	mDissolve(nullptr),
	mItem(nullptr),
	mbOpen(false)
{
	SetTypeID<CD2ChestObject>();
}

CD2ChestObject::CD2ChestObject(const CD2ChestObject& com)	:
	CD2ClickableObject(com)
{
	mbOpen = false;

	if (!mSprite)
	{
		mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
	}

	if (!mDissolve)
	{
		mDissolve = mObject->FindObjectComponentFromType<CDissolveComponent>();
	}

	if (!mItem)
	{
		mItem = CD2ItemTable::GetInst()->GenerateRandomItem();
	}
}

CD2ChestObject::~CD2ChestObject()
{
}

bool CD2ChestObject::Init()
{
	CD2ClickableObject::Init();
	
	return true;
}

void CD2ChestObject::Start()
{
	CD2ClickableObject::Start();

	if (!mSprite)
	{
		mSprite = mObject->FindSceneComponentFromType<CSpriteComponent>();
		mSprite->GetAnimationInstance()->Stop();
	}

	if (!mDissolve)
	{
		mDissolve = mObject->FindObjectComponentFromType<CDissolveComponent>();
	}

	if (!mItem)
	{
		mItem = CD2ItemTable::GetInst()->GenerateRandomItem();
	}

	mDissolve->SetInverse(true);
	mDissolve->SetInLineColor(1.f, 1.f, 1.f, 1.f);
	mDissolve->SetCenterLineColor(0.63f, 0.59f, 1.f, 1.f);
	mDissolve->SetOutLineColor(0.17f, 0.1f, 0.76f, 1.f);
	mDissolve->SetMaterial(mSprite->GetMaterial());
	mDissolve->SetFinishTime(2.f);
	mDissolve->StartDissolve();

	static_cast<CD2ClickableObjectWindow*>(mUI->GetWidgetWindow())->SetText("Chest");
}

void CD2ChestObject::Update(float deltaTime)
{
	CD2ClickableObject::Update(deltaTime);
}

void CD2ChestObject::PostUpdate(float deltaTime)
{
	CD2ClickableObject::PostUpdate(deltaTime);
}

void CD2ChestObject::PrevRender()
{
	CD2ClickableObject::PrevRender();
}

void CD2ChestObject::Render()
{
	CD2ClickableObject::Render();
}

void CD2ChestObject::PostRender()
{
	CD2ClickableObject::PostRender();
}

CD2ChestObject* CD2ChestObject::Clone()
{
	return new CD2ChestObject(*this);
}

void CD2ChestObject::OnClick()
{
	// 이전에 이미 한 번 열렸다면 
	if (mbOpen)
	{
		return;
	}

	mbOpen = true;
	// 이 오브젝트 주위 탐색해서 주위 타일에 아이템 오브젝트 생성
	Vector3 ownerPos = mObject->GetWorldPos();
	Vector3 aroundPos = mObject->GetScene()->GetNavigationManager()->GetAroundEmptyTile(ownerPos);

	if (aroundPos == Vector3::Zero)
	{
		return;
	}

	CGameObject* itemObj = CD2ObjectPool::GetInst()->ActiveInteractionObject("ItemObjTemplate", aroundPos);
	itemObj->Enable(true);
	itemObj->Start();
	static_cast<CD2ItemObjectComponent*>(itemObj->FindObjectComponentFromType<CD2ItemObjectComponent>())->SetItem(mItem);

	// 열리는 애니메이션 재생
	mSprite->GetAnimationInstance()->Play();

	// Sound
	mObject->GetScene()->GetResource()->SoundPlay("Chest");
}
