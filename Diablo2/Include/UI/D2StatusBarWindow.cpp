#include "D2StatusBarWindow.h"
#include "D2InventoryWindow.h"
#include "Resource/Excel/ExcelData.h"
#include "Scene/Viewport.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "../Component/D2Inventory.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "../Component/D2PlayerSkillComponent.h"
#include "../Component/D2Item.h"
#include "../Component/D2ItemTable.h"
#include "../Component/D2UIManager.h"
#include "../Component/D2DataManager.h"
#include "D2MouseItemSelect.h"
#include "Scene/SceneResource.h"
#include "Resource/Sound/Sound.h"

CD2StatusBarWindow::CD2StatusBarWindow()
{
}

CD2StatusBarWindow::CD2StatusBarWindow(const CD2StatusBarWindow& window) :
	CWidgetWindow(window)
{
}

CD2StatusBarWindow::~CD2StatusBarWindow()
{
}

bool CD2StatusBarWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2StatusBarWindow::Start()
{
	CWidgetWindow::Start();

	mInven = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2Inventory>();
	mCharInfo = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2CharacterInfoComponent>();
	mSkill = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2PlayerSkillComponent>();

	mCharInfo->SetEventCallBack("HpChange", this, &CD2StatusBarWindow::onHpChange, eD2CharInfoEventType::HpDec);
	mCharInfo->SetEventCallBack("HpChange", this, &CD2StatusBarWindow::onHpChange, eD2CharInfoEventType::HpInc);
	mCharInfo->SetEventCallBack("HpChange", this, &CD2StatusBarWindow::onHpChange, eD2CharInfoEventType::MaxHpDec);
	mCharInfo->SetEventCallBack("HpChange", this, &CD2StatusBarWindow::onHpChange, eD2CharInfoEventType::MaxHpInc);
	mCharInfo->SetEventCallBack("HpChange", this, &CD2StatusBarWindow::onHpChange, eD2CharInfoEventType::CCStart);
	mCharInfo->SetEventCallBack("HpChange", this, &CD2StatusBarWindow::onHpChange, eD2CharInfoEventType::CCEnd);

	mCharInfo->SetEventCallBack("MpChange", this, &CD2StatusBarWindow::onMpChange, eD2CharInfoEventType::MpDec);
	mCharInfo->SetEventCallBack("MpChange", this, &CD2StatusBarWindow::onMpChange, eD2CharInfoEventType::MpInc);
	mCharInfo->SetEventCallBack("MpChange", this, &CD2StatusBarWindow::onMpChange, eD2CharInfoEventType::MaxMpDec);
	mCharInfo->SetEventCallBack("MpChange", this, &CD2StatusBarWindow::onMpChange, eD2CharInfoEventType::MaxMpInc);

	mCharInfo->SetEventCallBack("ExpChange", this, &CD2StatusBarWindow::onExpChange, eD2CharInfoEventType::ExpUp);
	mCharInfo->SetEventCallBack("ActiveOpenSkilltree", this, &CD2StatusBarWindow::onLevelUp, eD2CharInfoEventType::LevelUp);
	
	SetRSkillButtonTexture(mSkill->GetCurrentSkillName());
}

void CD2StatusBarWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2StatusBarWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2StatusBarWindow::Render()
{
	CWidgetWindow::Render();
}

CD2StatusBarWindow* CD2StatusBarWindow::Clone()
{
	return new CD2StatusBarWindow(*this);
}

void CD2StatusBarWindow::MakeWidget(CExcelData* data)
{	
	// Create Widget
	mStatusBarPanel = CreateWidget<CImage>("StatusBarPanel");
	mHpBar = CreateWidget<CProgressBar>("HpBar");
	mMpBar = CreateWidget<CProgressBar>("MpBar");
	mExpBar = CreateWidget<CProgressBar>("ExpBar");
	mStaminaBar = CreateWidget<CProgressBar>("StaminaBar");
	mQuickSlot = CreateWidget<CGrid>("QuickSlot");
	mLeftSkillButton = CreateWidget<CButton>("LeftSkill");
	mRightSkillButton = CreateWidget<CButton>("RightSkill");
	mOpenSkillTreeButton = CreateWidget<CButton>("OpenSkillTree");

	for (int i = 0; i < 4; ++i)
	{ 
		mQuickSlotImage[i] = CreateWidget<CImage>("QuickSlotImage" + std::to_string(i));
		mQuickSlotImage[i]->Enable(false);
		mQuickSlotImage[i]->SetTextureTint(Vector4(0.f, 0.f, 0.f, 0.f));
		mQuickSlotImage[i]->SetZorder(1);
	}

	mQuickSlot->SetZorder(2);

	// Position, Size
	std::vector<std::string>* row = data->GetRow("StatusBarPanel");
	mStatusBarPanel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mStatusBarPanel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("HpBar");
	mHpBar->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mHpBar->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("MpBar");
	mMpBar->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mMpBar->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("ExpBar");
	mExpBar->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mExpBar->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("StaminaBar");
	mStaminaBar->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mStaminaBar->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("QuickSlot");
	mQuickSlot->SetCellCount(4, 1);
	mQuickSlot->SetCellSize(31.f, 31.f);
	mQuickSlot->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("RightSkillButton");
	mRightSkillButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mRightSkillButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("LeftSkillButton");
	mLeftSkillButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mLeftSkillButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("OpenSkillTreeButton");
	mOpenSkillTreeButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mOpenSkillTreeButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
	mStatusBarPanel->SetTexture("StatusBarPanel", TEXT("Dia2UI/StatusBar.png"));
	mHpBar->SetTexture("HpBar", TEXT("Dia2UI/HPBall.png"));
	mMpBar->SetTexture("MpBar", TEXT("Dia2UI/MPBall.png"));
	mExpBar->SetTextureTint(Vector4(1.f, 0.5f, 0.f, 1.f));
	mExpBar->SetDir(eProgressBarDir::RightToLeft);
	mExpBar->SetPercent(0.f);
	mStaminaBar->SetTextureTint(Vector4(0.9f, 0.8f, 0.f, 1.f));
	mQuickSlot->SetTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
	mQuickSlot->SetOccupiedTint(Vector4(0.f, 0.f, 1.f, 0.4f));
	mHpBar->SetDir(eProgressBarDir::TopToBottom);
	mMpBar->SetDir(eProgressBarDir::TopToBottom);
	mLeftSkillButton->SetTexture(eButtonState::Disabled, "LeftSkillButton", TEXT("Dia2UI/SkillIcon/0032.bmp"));
	mLeftSkillButton->ButtonEnable(false);
	mOpenSkillTreeButton->SetTexture(eButtonState::Normal, "UpButton", TEXT("Dia2UI/UpButton.png"));
	mOpenSkillTreeButton->SetTexture(eButtonState::Hovered, "UpButton", TEXT("Dia2UI/UpButton.png"));
	mOpenSkillTreeButton->SetTexture(eButtonState::LClicked, "UpButton", TEXT("Dia2UI/UpButton.png"));
	mOpenSkillTreeButton->SetTexture(eButtonState::Disabled, "UpButton", TEXT("Dia2UI/UpButton.png"));
	mOpenSkillTreeButton->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(31.f, 30.f));
	mOpenSkillTreeButton->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(31.f, 30.f));
	mOpenSkillTreeButton->AddFrameData(eButtonState::LClicked, Vector2(31.f, 0.f), Vector2(31.f, 30.f));
	mOpenSkillTreeButton->AddFrameData(eButtonState::Disabled, Vector2(61.f, 0.f), Vector2(31.f, 30.f));
	mOpenSkillTreeButton->ButtonEnable(false);

	// CallBack
	mQuickSlot->SetCallBack(eButtonState::LClicked, this, &CD2StatusBarWindow::onLClickQuickSlot);
	mQuickSlot->SetCallBack(eButtonState::RClicked, this, &CD2StatusBarWindow::onRClickQuickSlot);
	mRightSkillButton->SetLClickCallBack(this, &CD2StatusBarWindow::onClickRSkillButton);
	mOpenSkillTreeButton->SetLClickCallBack(this, &CD2StatusBarWindow::onClickOpenSkillTree);
}

void CD2StatusBarWindow::SetRSkillButtonTexture(const std::string& skillName)
{
	CTexture* texture = CD2DataManager::GetInst()->GetSkillIconTexture(skillName);

	mRightSkillButton->SetTexture(eButtonState::Normal, texture);
	mRightSkillButton->SetTexture(eButtonState::Hovered, texture);
	mRightSkillButton->SetTexture(eButtonState::LClicked, texture);
	mRightSkillButton->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mRightSkillButton->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mRightSkillButton->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
}

void CD2StatusBarWindow::SetQuickSlotTintEnable(bool bEnable)
{
	if (bEnable)
	{
		mQuickSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.3f));
	}
	else
	{
		mQuickSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));
	}
}

void CD2StatusBarWindow::onHpChange()
{
	float hp = mCharInfo->GetHp();
	float maxHP = mCharInfo->GetMaxHp();
	mHpBar->SetPercent(hp / maxHP);

	eD2ElementType type = mCharInfo->GetCurCC();

	if (type == eD2ElementType::Poison)
	{
		mHpBar->SetTextureTint(Vector4(0.f, 1.f, 0.f, 1.f));
	}
	else
	{
		mHpBar->SetTextureTint(Vector4(1.f, 1.f, 1.f, 1.f));
	}
}

void CD2StatusBarWindow::onMpChange()
{
	float mp = mCharInfo->GetMp();
	float maxMP = mCharInfo->GetMaxMp();
	mMpBar->SetPercent(mp / maxMP);
}

void CD2StatusBarWindow::onExpChange()
{
	mExpBar->SetPercent(mCharInfo->GetExpPercent());
}

void CD2StatusBarWindow::onLevelUp()
{
	mOpenSkillTreeButton->ButtonEnable(true);
}

void CD2StatusBarWindow::OnRemainSkillPointZero()
{
	mOpenSkillTreeButton->ButtonEnable(false);
}

void CD2StatusBarWindow::onClickRSkillButton()
{
	CD2UIManager::GetInst()->ToggleMenu(eD2MenuWindowType::SkillSelect);
}

void CD2StatusBarWindow::onClickOpenSkillTree()
{
	CD2UIManager::GetInst()->ToggleMenu(eD2MenuWindowType::SkillTree);
}

void CD2StatusBarWindow::onLClickQuickSlot(int idx)
{
	CD2Item* grabItem = mInven->GetGrabItem();

	// 현재 들고 있는 아이템이 있는 경우
	if (grabItem)
	{
		// Usable 한 아이템이 아닌 경우 상호작용 없음
		if (eD2ItemUseType::Usable != grabItem->GetItemUseType())
		{
			return;
		}

		CD2Item* slotItem = mInven->GetQuickSlotItem(idx);
		mInven->UnGrabQuickSlotItem(idx);

		// Grid UI 처리
		mQuickSlot->OccupyCell(idx);
		
		// Image 처리
		setQuickSlotTexture(idx, grabItem);
	
		// 교체된 아이템이 있는 경우
		if (mInven->GetGrabItem())
		{
			CTexture* mouseTexture = CD2ItemTable::GetInst()->FindItemUITexture(mInven->GetGrabItem());
			CEngine::GetInst()->SetMouseState(eMouseState::State1);
			CD2MouseItemSelect* mouse = static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget());
			mouse->SetMouseTexture(mouseTexture);

			CD2UIManager::GetInst()->GetInventoryWindow()->SetItemSlotTintEnable(true);
		}
		else
		{
			CEngine::GetInst()->SetMouseState(eMouseState::Normal);
			CD2UIManager::GetInst()->GetInventoryWindow()->SetItemSlotTintEnable(false);
		}
	}
	// 현재 들고 있는 아이템이 없는 경우
	else
	{
		CD2Item* slotItem = mInven->GetQuickSlotItem(idx);

		// 현재 슬롯에 아이템이 있는 경우
		if (slotItem)
		{
			mInven->GrabQuickSlotItem(idx);

			mQuickSlot->UnOccupyCell(idx);

			mQuickSlotImage[idx]->Enable(false);

			CTexture* mouseTexture = CD2ItemTable::GetInst()->FindItemUITexture(mInven->GetGrabItem());
			CEngine::GetInst()->SetMouseState(eMouseState::State1);
			CD2MouseItemSelect* mouse = static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget());
			mouse->SetMouseTexture(mouseTexture);
			CD2UIManager::GetInst()->GetInventoryWindow()->SetItemSlotTintEnable(true);
		}
	}

	mViewport->GetScene()->GetResource()->SoundPlay("PotionUI");
}

void CD2StatusBarWindow::onRClickQuickSlot(int idx)
{
	if (mInven->UseQuickSlotItem(idx))
	{
		mQuickSlot->UnOccupyCell(idx);
		mQuickSlotImage[idx]->Enable(false);
		SetQuickSlotTintEnable(false);
		CD2UIManager::GetInst()->GetInventoryWindow()->SetItemSlotTintEnable(false);
		mViewport->GetScene()->GetResource()->SoundPlay("UsePotion");
	}
}

void CD2StatusBarWindow::setQuickSlotTexture(const int idx, CD2Item* item)
{
	mQuickSlotImage[idx]->Enable(true);
	CTexture* texture = CD2ItemTable::GetInst()->FindItemUITexture(item);

	mQuickSlotImage[idx]->SetTexture(texture);
	mQuickSlotImage[idx]->AddFrameData(Vector2(0.f, 0.f), Vector2(texture->GetWidth(), texture->GetHeight()));
	mQuickSlotImage[idx]->SetTextureTint(Vector4(1.f, 1.f, 1.f, 1.f));
	mQuickSlotImage[idx]->SetSize(texture->GetWidth(), texture->GetHeight());
	mQuickSlotImage[idx]->Enable(true);

	// 이미지 위치 : 아이템 슬롯 기준 인덱스 셀에서 가장 왼쪽 아래
	int idxX = idx % INVEN_COUNTX;
	int idxY = idx / INVEN_COUNTX;
	int lbIdx = (idxY + item->GetCellHeight() - 1) * INVEN_COUNTX + idxX;

	Vector2 gridPos = mQuickSlot->GetWidgetPos();
	Vector2 cellPos = mQuickSlot->GetCellPos(lbIdx);

	mQuickSlotImage[idx]->SetPos(gridPos + cellPos);
}
