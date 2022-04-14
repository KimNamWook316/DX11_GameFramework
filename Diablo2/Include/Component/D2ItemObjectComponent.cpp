#include "D2ItemObjectComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "D2Inventory.h"
#include "D2ItemTable.h"
#include "D2ObjectPool.h"
#include "../UI/D2MouseItemSelect.h"
#include "../UI/D2InventoryWindow.h"
#include "../UI/D2ClickableObjectWindow.h"
#include "D2ClickableUIComponent.h"
#include "D2UIManager.h"
#include "D2StateComponent.h"

CD2ItemObjectComponent::CD2ItemObjectComponent()	:
	mItem(nullptr),
	mSprite(nullptr)
{
	SetTypeID<CD2ItemObjectComponent>();
}

CD2ItemObjectComponent::CD2ItemObjectComponent(const CD2ItemObjectComponent& com)	:
	CD2ClickableObject(com)
{
	mItem = nullptr;
}

CD2ItemObjectComponent::~CD2ItemObjectComponent()
{
}

bool CD2ItemObjectComponent::Init()
{
	return true;
}

void CD2ItemObjectComponent::Start()
{
	CD2ClickableObject::Start();

	if (!mSprite)
	{
		mSprite = (CSpriteComponent*)mObject->GetRootSceneComponent();
	}
}

void CD2ItemObjectComponent::Update(float deltaTime)
{
}

void CD2ItemObjectComponent::PostUpdate(float deltaTime)
{
}

void CD2ItemObjectComponent::PrevRender()
{
}

void CD2ItemObjectComponent::Render()
{
}

void CD2ItemObjectComponent::PostRender()
{
}

CD2ItemObjectComponent* CD2ItemObjectComponent::Clone()
{
	return new CD2ItemObjectComponent(*this);
}

void CD2ItemObjectComponent::OnClick()
{
	CD2Inventory* playerInven = mObject->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2Inventory>();

	// ���� Grab�� �������� ���� ��쿡 �ֿ� �� �ִ�.
	if (!playerInven->GetGrabItem())
	{
		// �κ��丮���� ���� ������Ʈ ������ Grab ó��
		playerInven->GrabItem(mItem);
		CD2Item* grabItem = playerInven->GetGrabItem();

		// Item Owner ����
		grabItem->SetOwner(mScene->GetPlayerObj());

		CEngine::GetInst()->SetMouseState(eMouseState::State1);
		static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget())->
			SetMouseTexture(CD2ItemTable::GetInst()->FindItemUITexture(grabItem));

		// �κ��丮 UI �׸��� ���� ���� ����
		CD2UIManager::GetInst()->GetInventoryWindow()->SetItemSlotTintEnable(true, grabItem->GetCellWidth(), grabItem->GetCellHeight());

		// ������Ʈ ��ȯ
		CD2ObjectPool::GetInst()->ReturnInteractionObject("ItemObjTemplate", mObject);
	}
}

void CD2ItemObjectComponent::SetItem(class CD2Item* item)
{
	mItem = item;

	if (!mSprite)
	{
		mSprite = (CSpriteComponent*)mObject->GetRootSceneComponent();
	}

	mSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	std::string itemObjSequenceName = CD2ItemTable::GetInst()->FindItemObjAnimSequenceName(item);
	mSprite->GetAnimationInstance()->AddAnimation(itemObjSequenceName, "ItemAnim", false, 0.7f, 1.0f);

	CAnimationSequence2D* anim = mSprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence();
	CTexture* tex = anim->GetTexture();
	mObject->SetWorldScale(tex->GetWidth() / anim->GetFrameCount(), tex->GetHeight(), 1.f);
	mSprite->GetAnimationInstance()->Play();

	// UI
	static_cast<CD2ClickableObjectWindow*>(mUI->GetWidgetWindow())->SetText(item->GetName());

	// Sound
	mObject->GetScene()->GetResource()->SoundPlay("ItemDrop");
}
