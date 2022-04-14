#include "D2InventoryWindow.h"
#include "D2StatusBarWindow.h"
#include "Scene/Viewport.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "Resource/Excel/ExcelData.h"
#include "../Component/D2Inventory.h"
#include "../Component/D2UIManager.h"
#include "../Component/D2ItemTable.h"
#include "D2MouseItemSelect.h"
#include "Engine.h"
#include "../Component/D2UIManager.h"

CD2InventoryWindow::CD2InventoryWindow()	:
	mInven(nullptr)
{
}

CD2InventoryWindow::CD2InventoryWindow(const CD2InventoryWindow& window)	:
	CWidgetWindow(window)
{
}

CD2InventoryWindow::~CD2InventoryWindow()
{
}

bool CD2InventoryWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2InventoryWindow::Start()
{
	CWidgetWindow::Start();
	
	mInven = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2Inventory>();

	// ó�� ������ ���� ��, ���� �κ��丮 ���� ����ȭ
	std::vector<CD2Item*> equipmentSlotInfo;
	mInven->GetEquipmentSlotState(equipmentSlotInfo);
	std::vector<D2ItemSlotInfo> invenSlotInfo;
	mInven->GetInventorySlotState(invenSlotInfo);

	size_t size = equipmentSlotInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (equipmentSlotInfo[i])
		{
			SetEquipmentTexture((eD2ItemType)i, equipmentSlotInfo[i]);
		}
	}

	size = invenSlotInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		SetItemSlotTexture(invenSlotInfo[i].SlotRootIndex, invenSlotInfo[i].Item);
	}
}

void CD2InventoryWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2InventoryWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2InventoryWindow::Render()
{
	CWidgetWindow::Render();
}

CD2InventoryWindow* CD2InventoryWindow::Clone()
{
	return new CD2InventoryWindow(*this);
}

void CD2InventoryWindow::SetItemSlotTintEnable(bool bEnable, const int selectWidth, const int selectHeight)
{
	if (bEnable)
	{
		mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.3f));
	}
	else
	{
		mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));
	}

	mItemSlot->SetSelectRange(selectWidth, selectHeight);
}

void CD2InventoryWindow::MakeWidget(CExcelData* data)
{
	// Create Widget
	mInvenPanel = CreateWidget<CImage>("Panel");
	mItemSlot = CreateWidget<CGrid>("ItemSlot");
	mCloseButton = CreateWidget<CButton>("CloseButton");
	mEqipmentSlot[(int)eD2ItemType::Head] = CreateWidget<CButton>("HeadSlot");
	mEqipmentSlot[(int)eD2ItemType::MainWeapon] = CreateWidget<CButton>("MainWeaponSlot");
	mEqipmentSlot[(int)eD2ItemType::SubWeapon] = CreateWidget<CButton>("SubWeaponSlot");
	mEqipmentSlot[(int)eD2ItemType::Armor] = CreateWidget<CButton>("ArmorSlot");
	mEqipmentSlot[(int)eD2ItemType::Belt] = CreateWidget<CButton>("BeltSlot");
	mEqipmentSlot[(int)eD2ItemType::Boots] = CreateWidget<CButton>("BootsSlot");
	mEqipmentSlot[(int)eD2ItemType::Globe] = CreateWidget<CButton>("GlobeSlot");
	
	// Create Slot Image Widget
	// 40���� ���� �̹��� �̸� �����ΰ�, �̹��� ��ü�Ͽ� ����Ѵ�.
	mVecItemSlotImage.resize(INVEN_COUNTX * INVEN_COUNTY);

	for (int i = 0; i < INVEN_COUNTX * INVEN_COUNTY; ++i)
	{
		mVecItemSlotImage[i] = CreateWidget<CImage>("InvenSlotImage" + std::to_string(i));
		mVecItemSlotImage[i]->Enable(false);
		mVecItemSlotImage[i]->SetTextureTint(Vector4(0.f, 0.f, 0.f, 0.f));
		mVecItemSlotImage[i]->SetZorder(1);
	}

	mItemSlot->SetZorder(2);

	// Position, Size
	std::vector<std::string>* row = data->GetRow("Panel");
	mInvenPanel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mInvenPanel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("ItemSlot");
	mItemSlot->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	mItemSlot->SetCellSize(29, 29);
	mItemSlot->SetCellCount(10, 4);

	row = data->GetRow("CloseButton");
	mCloseButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mCloseButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("HeadSlot");
	mEqipmentSlot[(int)eD2ItemType::Head]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::Head]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("MainWeaponSlot");
	mEqipmentSlot[(int)eD2ItemType::MainWeapon]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::MainWeapon]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("SubWeaponSlot");
	mEqipmentSlot[(int)eD2ItemType::SubWeapon]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::SubWeapon]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("ArmorSlot");
	mEqipmentSlot[(int)eD2ItemType::Armor]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::Armor]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("BootsSlot");
	mEqipmentSlot[(int)eD2ItemType::Boots]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::Boots]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("BeltSlot");
	mEqipmentSlot[(int)eD2ItemType::Belt]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::Belt]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("GlobeSlot");
	mEqipmentSlot[(int)eD2ItemType::Globe]->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEqipmentSlot[(int)eD2ItemType::Globe]->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
	mInvenPanel->SetTexture("InvenPanel", TEXT("Dia2UI/inven_panel.png"));
	mCloseButton->SetTexture(eButtonState::Normal, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mCloseButton->SetTexture(eButtonState::Hovered, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mCloseButton->SetTexture(eButtonState::LClicked, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mCloseButton->AddFrameData(eButtonState::Normal, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mCloseButton->AddFrameData(eButtonState::Hovered, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mCloseButton->AddFrameData(eButtonState::LClicked, Vector2(352.f, 0.f), Vector2(32.f, 32.f));
	mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.0f));
	mItemSlot->SetTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
	mItemSlot->SetOccupiedTint(Vector4(0.f, 0.f, 1.f, 0.4f));

	for (int i = 0; i < (int)eD2ItemType::Etc; ++i)
	{
		mEqipmentSlot[i]->SetTextureTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
		mEqipmentSlot[i]->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.0f));
		mEqipmentSlot[i]->SetTextureTint(eButtonState::LClicked, Vector4(0.f, 0.f, 0.f, 0.0f));
	}

	// CallBack
	mCloseButton->SetLClickCallBack(this, &CD2InventoryWindow::onClickClose);
	mItemSlot->SetCallBack(eButtonState::LClicked, this, &CD2InventoryWindow::onLClickItemSlot);
	mItemSlot->SetCallBack(eButtonState::RClicked, this, &CD2InventoryWindow::onRClickItemSlot);
	mEqipmentSlot[(int)eD2ItemType::Head]->SetLClickCallBack(this, &CD2InventoryWindow::onClickHeadSlot);
	mEqipmentSlot[(int)eD2ItemType::Armor]->SetLClickCallBack(this, &CD2InventoryWindow::onClickArmorSlot);
	mEqipmentSlot[(int)eD2ItemType::MainWeapon]->SetLClickCallBack(this, &CD2InventoryWindow::onClickMainWeaponSlot);
	mEqipmentSlot[(int)eD2ItemType::SubWeapon]->SetLClickCallBack(this, &CD2InventoryWindow::onClickSubWeaponSlot);
	mEqipmentSlot[(int)eD2ItemType::Belt]->SetLClickCallBack(this, &CD2InventoryWindow::onClickBeltSlot);
	mEqipmentSlot[(int)eD2ItemType::Globe]->SetLClickCallBack(this, &CD2InventoryWindow::onClickGlobeSlot);
	mEqipmentSlot[(int)eD2ItemType::Boots]->SetLClickCallBack(this, &CD2InventoryWindow::onClickBootsSlot);

	mEqipmentSlot[(int)eD2ItemType::Head]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverHeadSlot);
	mEqipmentSlot[(int)eD2ItemType::Armor]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverArmorSlot);
	mEqipmentSlot[(int)eD2ItemType::MainWeapon]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverMainWeaponSlot);
	mEqipmentSlot[(int)eD2ItemType::SubWeapon]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverSubWeaponSlot);
	mEqipmentSlot[(int)eD2ItemType::Belt]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverBeltSlot);
	mEqipmentSlot[(int)eD2ItemType::Globe]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverGlobeSlot);
	mEqipmentSlot[(int)eD2ItemType::Boots]->SetHoverCallBack(this, &CD2InventoryWindow::onHoverBootsSlot);
}

void CD2InventoryWindow::SetEquipmentTexture(eD2ItemType type, CD2Item* item)
{
	CTexture* texture = CD2ItemTable::GetInst()->FindItemUITexture(item);

	if (!texture)
	{
		assert(false); 
		return;
	}

	mEqipmentSlot[(int)type]->SetTexture(eButtonState::Normal, texture);
	mEqipmentSlot[(int)type]->SetTexture(eButtonState::Hovered, texture);
	mEqipmentSlot[(int)type]->SetTexture(eButtonState::LClicked, texture);
	mEqipmentSlot[(int)type]->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(texture->GetWidth(), texture->GetHeight()));
	mEqipmentSlot[(int)type]->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(texture->GetWidth(), texture->GetHeight()));
	mEqipmentSlot[(int)type]->AddFrameData(eButtonState::LClicked, Vector2(0.f, 0.f), Vector2(texture->GetWidth(), texture->GetHeight()));
	mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 1.f));
	mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::Normal, Vector4(1.f, 1.f, 1.f, 1.f));
	mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::LClicked, Vector4(1.f, 1.f, 1.f, 1.f));
}

void CD2InventoryWindow::SetItemSlotTexture(const int rootIdx, CD2Item* item)
{
	CTexture* texture = CD2ItemTable::GetInst()->FindItemUITexture(item);

	mVecItemSlotImage[rootIdx]->SetTexture(texture);
	mVecItemSlotImage[rootIdx]->AddFrameData(Vector2(0.f, 0.f), Vector2(texture->GetWidth(), texture->GetHeight()));
	mVecItemSlotImage[rootIdx]->SetTextureTint(Vector4(1.f, 1.f, 1.f, 1.f));
	mVecItemSlotImage[rootIdx]->SetSize(texture->GetWidth(), texture->GetHeight());
	mVecItemSlotImage[rootIdx]->Enable(true);

	// �̹��� ��ġ : ������ ���� ���� �ε��� ������ ���� ���� �Ʒ�
	int idxX = rootIdx % INVEN_COUNTX;
	int idxY = rootIdx / INVEN_COUNTX;
	int lbIdx = (idxY + item->GetCellHeight() - 1) * INVEN_COUNTX + idxX;

	Vector2 gridPos = mItemSlot->GetWidgetPos();
	Vector2 cellPos = mItemSlot->GetCellPos(lbIdx);

	mVecItemSlotImage[rootIdx]->SetPos(gridPos + cellPos);

	for (int y = 0; y < item->GetCellHeight(); ++y)
	{
		for (int x = 0; x < item->GetCellWidth(); ++x)
		{
			mItemSlot->OccupyCell((y + idxY) * INVEN_COUNTX + (idxX + x));
		}
	}
}

void CD2InventoryWindow::onClickClose()
{
	CD2UIManager::GetInst()->CloseMenu();
}

void CD2InventoryWindow::onLClickItemSlot(int idx)
{
	// ���� ��� �ִ� �������� �ִ� ���
	if (mInven->GetGrabItem())
	{
		// ���� ��� �ִ� ������ ( ��ü�� ������ )
		CD2Item* grabItem = mInven->GetGrabItem();

		// ���� ��� �ִ� �����ۿ� �������� �������� �ִ��� üũ
		int overlapItemRoot = mInven->GetOverlappedItemRootIndex(idx);

		// �������� �������� �����ϰ� �ִ� �� �ε���
		std::vector<int> vecOverlapIndexes;

		if (-1 != overlapItemRoot)
		{
			mInven->GetItemOccupiedIndexes(overlapItemRoot, vecOverlapIndexes);
		}

		// Ungrab
		bool bUnGrab = mInven->UnGrabItem(idx);

		// ���� ������ ���
		if (bUnGrab)
		{
			// �׸��� occupy ó��
			std::vector<int> occupyIndexes;
			mInven->GetItemOccupiedIndexes(idx, occupyIndexes);
			mItemSlot->OccupyCell(occupyIndexes);
			
			// UnGrab�� �ڸ��� �ٸ� �������� ������ ���
			if (!mInven->GetGrabItem())
			{
				// ������ ���� �̹���
				mVecItemSlotImage[idx]->Enable(true);
				SetItemSlotTexture(idx, grabItem);

				// ���콺 ��ü
				CEngine::GetInst()->SetMouseState(eMouseState::Normal);

				// �׸��� ���� ���� 1x1��, Hovered ���� ���ֱ�
				mItemSlot->SetSelectRange(1, 1);
				mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));

				// ������ Hovered ����
				CD2UIManager::GetInst()->GetStatusBarWindow()->SetQuickSlotTintEnable(false);

				// Sound
				playInvenSound(grabItem->GetItemType());
			}
			// �ٸ� �������� �־ GrabItem�� �ٲ���� ���
			else
			{
				// ������ ������ �̹��� ����
				mVecItemSlotImage[overlapItemRoot]->Enable(false);

				// ������ ���� ���� ������ �̹����� ��� False
				int cellCountX = mItemSlot->GetCellCountX();
				int idxX = idx % cellCountX;
				int idxY = idx / cellCountX;

				int rangeX = grabItem->GetCellWidth();
				int rangeY = grabItem->GetCellHeight();

				for (int y = 0; y < rangeY; ++y)
				{
					for (int x = 0; x < rangeX; ++x)
					{
						mVecItemSlotImage[(idxY + y) * cellCountX + (idxX + x)]->Enable(false);
					}
				}

				// �ٲٱ� ���� �������� �����ϰ� �ִ� �׸��� �� ���� ������ ������ ��ġ�� ���� ���� Normal ���·� �ǵ�����.
				{
					int minX = idxX;
					int minY = idxY;
					int maxX = idxX + rangeX - 1;
					int maxY = idxY + rangeY - 1;

					size_t size = vecOverlapIndexes.size();
					for (size_t i = 0; i < size; ++i)
					{
						int index = vecOverlapIndexes[i];
						int curIdxX = index % cellCountX;
						int curIdxY = index / cellCountX;

						if (curIdxX < minX || curIdxX > maxX || curIdxY < minY || curIdxY > maxY)
						{
							mItemSlot->UnOccupyCell(index);
							continue;
						}
					}
				}

				// ������ ���� �̹���
				mVecItemSlotImage[idx]->Enable(true);
				SetItemSlotTexture(idx, grabItem);

				mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.3f));
				mItemSlot->SetSelectRange(mInven->GetGrabItem()->GetCellWidth(), mInven->GetGrabItem()->GetCellHeight());

				CTexture* mouseTexture = CD2ItemTable::GetInst()->FindItemUITexture(mInven->GetGrabItem());
				CEngine::GetInst()->SetMouseState(eMouseState::State1);
				CD2MouseItemSelect* mouse = static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget());
				mouse->SetMouseTexture(mouseTexture);

				// �Һ� �������̶��, �����Ե� Tintó���Ѵ�.
				if (eD2ItemUseType::Usable == mInven->GetGrabItem()->GetItemUseType())
				{
					CD2UIManager::GetInst()->GetStatusBarWindow()->SetQuickSlotTintEnable(true);
				}

				// Sound
				playInvenSound(mInven->GetGrabItem()->GetItemType());
			}
		}
	}
	else
	{
		// ���� �κ��丮 �ش� �ε����� �������� ���� ���
		if (!mInven->GetItem(idx))
		{
			return;
		}

		int rootIdx = mInven->GetRootSlotIndex(idx);
		std::vector<int> occupyIndexes;
		mInven->GetItemOccupiedIndexes(rootIdx, occupyIndexes);

		// �κ��丮���� Grab
		mInven->GrabItem(idx);

		// UI
		mVecItemSlotImage[rootIdx]->Enable(false);
		mItemSlot->UnOccupyCell(occupyIndexes);

		// Grid ���� ����
		mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.3f));
		mItemSlot->SetSelectRange(mInven->GetGrabItem()->GetCellWidth(), mInven->GetGrabItem()->GetCellHeight());


		// �Һ� �������̶��, �����Ե� Tintó���Ѵ�.
		if (eD2ItemUseType::Usable == mInven->GetGrabItem()->GetItemUseType())
		{
			CD2UIManager::GetInst()->GetStatusBarWindow()->SetQuickSlotTintEnable(true);
		}

		// ���콺 ��ü
		CTexture* mouseTexture = CD2ItemTable::GetInst()->FindItemUITexture(mInven->GetGrabItem());
		CEngine::GetInst()->SetMouseState(eMouseState::State1);
		CD2MouseItemSelect* mouse = static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget());
		mouse->SetMouseTexture(mouseTexture);

		// Sound
		playInvenSound(mInven->GetGrabItem()->GetItemType());
	}
}

void CD2InventoryWindow::onRClickItemSlot(int idx)
{
	// ���� ��� �ִ� �������� ���� ��쿡�� ����
	if (!mInven->GetGrabItem())
	{
		CD2Item* clickedItem = mInven->GetItem(idx);

		if (!clickedItem)
		{
			return;
		}

		eD2ItemUseType useType = clickedItem->GetItemUseType();

		// ��� �������� ���
		if (eD2ItemUseType::Usable == useType)
		{
			bool bUse = mInven->UseItem(idx);

			if (bUse)
			{
				mVecItemSlotImage[idx]->Enable(false);
				mItemSlot->UnOccupyCell(idx);
			}
			
			// Sound
			mViewport->GetScene()->GetResource()->SoundPlay("UsePotion");
		}
		else if (eD2ItemUseType::Equip == useType)
		{
			// Grid ���� �ʱ�ȭ�� ����
			int clickItemRootIdx = mInven->GetRootSlotIndex(idx);
			std::vector<int> occupyIndexes;
			mInven->GetItemOccupiedIndexes(clickItemRootIdx, occupyIndexes);

			// ���� Ŭ���� �������� Ÿ���� ����, �ش� ���ĭ�� ��ü�� ��� �ִ���
			eD2ItemType equipType = clickedItem->GetItemType();
			CD2Item* changeEquipItem = mInven->GetEquipItem(equipType);

			bool bEquip = mInven->EquipItem(idx);

			if (bEquip)
			{
				// ���ĭ �ؽ��� ��ü
				SetEquipmentTexture(equipType, clickedItem);
				// ��ü�� �������� ���� ���, ũ��� �����Ƿ�, �̹����� ��ü���ش�.
				if (changeEquipItem)
				{
					SetItemSlotTexture(clickItemRootIdx, changeEquipItem);
				}
				// ��ü�� �������� ���� ���, ���Կ��� ����
				else
				{
					// UI
					mVecItemSlotImage[clickItemRootIdx]->Enable(false);
					mItemSlot->UnOccupyCell(occupyIndexes);
				}

				// Sound
				playInvenSound(equipType);
			}
		}
	}
}

void CD2InventoryWindow::onClickHeadSlot()
{
	clickEquipmentSlot(eD2ItemType::Head);
}

void CD2InventoryWindow::onClickArmorSlot()
{
	clickEquipmentSlot(eD2ItemType::Armor);
}

void CD2InventoryWindow::onClickMainWeaponSlot()
{
	clickEquipmentSlot(eD2ItemType::MainWeapon);
}

void CD2InventoryWindow::onClickSubWeaponSlot()
{
	clickEquipmentSlot(eD2ItemType::SubWeapon);
}

void CD2InventoryWindow::onClickBeltSlot()
{
	clickEquipmentSlot(eD2ItemType::Belt);
}

void CD2InventoryWindow::onClickGlobeSlot()
{
	clickEquipmentSlot(eD2ItemType::Globe);
}

void CD2InventoryWindow::onClickBootsSlot()
{
	clickEquipmentSlot(eD2ItemType::Boots);
}

void CD2InventoryWindow::onHoverHeadSlot()
{
	hoverEquipmentSlot(eD2ItemType::Head);
}

void CD2InventoryWindow::onHoverArmorSlot()
{
	hoverEquipmentSlot(eD2ItemType::Armor);
}

void CD2InventoryWindow::onHoverMainWeaponSlot()
{
	hoverEquipmentSlot(eD2ItemType::MainWeapon);
}

void CD2InventoryWindow::onHoverSubWeaponSlot()
{
	hoverEquipmentSlot(eD2ItemType::SubWeapon);
}

void CD2InventoryWindow::onHoverBeltSlot()
{
	hoverEquipmentSlot(eD2ItemType::Belt);
}

void CD2InventoryWindow::onHoverGlobeSlot()
{
	hoverEquipmentSlot(eD2ItemType::Globe);
}

void CD2InventoryWindow::onHoverBootsSlot()
{
	hoverEquipmentSlot(eD2ItemType::Boots);
}

void CD2InventoryWindow::clickEquipmentSlot(eD2ItemType type)
{
	// ���� ��� �ִ� �������� �ִ� ���
	if (mInven->GetGrabItem())
	{
		bool bEquip = mInven->UnGrabEquipItem(type);

		if (bEquip)
		{
			// ��ü�� �������� ���� ���
			if (mInven->GetGrabItem())
			{
				// ���콺 ��ü
				CTexture* mouseTexture = CD2ItemTable::GetInst()->FindItemUITexture(mInven->GetGrabItem());
				CEngine::GetInst()->SetMouseState(eMouseState::State1);
				CD2MouseItemSelect* mouse = static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget());
				mouse->SetMouseTexture(mouseTexture);

				// Grid ���� ����
				mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.3f));
				mItemSlot->SetSelectRange(mInven->GetGrabItem()->GetCellWidth(), mInven->GetGrabItem()->GetCellHeight());
			}
			// ��ü�� �������� ���� ���
			else
			{
				// ���콺 ��ü
				CEngine::GetInst()->SetMouseState(eMouseState::Normal);

				// �׸��� ���� ���� 1x1��, Hovered ���� ���ֱ�
				mItemSlot->SetSelectRange(1, 1);
				mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));
			}

			SetEquipmentTexture(type, mInven->GetEquipItem(type));
		}
	}
	// ���� ��� �ִ� �������� ���� ���
	else
	{
		// ���� ������ �������� �ִ� ���
		if (mInven->GetEquipItem(type))
		{
			mInven->GrabEquipItem(type);

			// Image
			mEqipmentSlot[(int)type]->SetTexture(eButtonState::Normal, nullptr);
			mEqipmentSlot[(int)type]->SetTexture(eButtonState::Hovered, nullptr);
			mEqipmentSlot[(int)type]->SetTexture(eButtonState::LClicked, nullptr);
			mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
			mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.3f));
			mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::LClicked, Vector4(0.f, 1.f, 0.f, 0.3f));
			mEqipmentSlot[(int)type]->SetUseTexture(false);

			// Grid ���� ���� ����
			mItemSlot->SetSelectRange(mInven->GetGrabItem()->GetCellWidth(), mInven->GetGrabItem()->GetCellHeight());
			mItemSlot->SetTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.4f));

			// Mouse
			CTexture* mouseTexture = CD2ItemTable::GetInst()->FindItemUITexture(mInven->GetGrabItem());
			CEngine::GetInst()->SetMouseState(eMouseState::State1);
			CD2MouseItemSelect* mouse = static_cast<CD2MouseItemSelect*>(CEngine::GetInst()->GetMouseWidget());
			mouse->SetMouseTexture(mouseTexture);
		}
	}

	playInvenSound(type);
}

void CD2InventoryWindow::hoverEquipmentSlot(eD2ItemType type)
{
	if (!mEqipmentSlot[(int)type])
	{
		return;
	}

	if (mInven->IsRightEquipmentSlot(type))
	{
		mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 1.f, 0.f, 0.8f));
	}
	else
	{
		mEqipmentSlot[(int)type]->SetTextureTint(eButtonState::Hovered, Vector4(1.f, 0.f, 0.f, 0.8f));
	}
}

void CD2InventoryWindow::playInvenSound(eD2ItemType type)
{
	CSound* sound = nullptr;
	switch (type)
	{
	case eD2ItemType::Head:
		sound = mViewport->GetScene()->GetResource()->FindSound("HeadEquip");
		break;
	case eD2ItemType::Armor:
		sound = mViewport->GetScene()->GetResource()->FindSound("ArmorEquip");
		break;
	case eD2ItemType::MainWeapon:
		sound = mViewport->GetScene()->GetResource()->FindSound("StaffEquip");
		break;
	case eD2ItemType::Belt:
		sound = mViewport->GetScene()->GetResource()->FindSound("BeltEquip");
		break;
	case eD2ItemType::Boots:
		sound = mViewport->GetScene()->GetResource()->FindSound("BootsEquip");
		break;
	case eD2ItemType::Etc:
		sound = mViewport->GetScene()->GetResource()->FindSound("PotionUI");
		break;
	}
	
	if (sound)
	{
		sound->Play();
	}
}
