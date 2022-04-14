#include "D2Inventory.h"
#include "D2Item.h"
#include "D2UsableItem.h"
#include "D2EquipItem.h"
#include "D2ObjectPool.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager.h"
#include "D2ItemObjectComponent.h"

#include "D2ItemTable.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"

CD2Inventory::CD2Inventory()    :
    mGrabItem(nullptr),
    mQuickSlot{},
    mEquipment{},
    mInvenSlot{}
{
    SetTypeID<CD2Inventory>();

    for (int i = 0; i < INVEN_COUNTX * INVEN_COUNTY; ++i)
    {
        SlotElement element;
        mInvenSlot[i] = element;
    }
}

CD2Inventory::CD2Inventory(const CD2Inventory& com) :
    CObjectComponent(com)
{
    mGrabItem = nullptr;
}

CD2Inventory::~CD2Inventory()
{
}

bool CD2Inventory::Init()
{
    return true;
}

void CD2Inventory::Start()
{
	// Test
	CD2Item* dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultArmor");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultHead");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultBelt");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultBoots");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultMainWeapon");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultMainWeapon");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("DefaultArmor");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("HpPortion");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }
    
	dummy = CD2ItemTable::GetInst()->GenerateItem("MpPortion");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("HpPortion");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("MpPortion");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

 	dummy = CD2ItemTable::GetInst()->GenerateItem("HpPortion");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }

	dummy = CD2ItemTable::GetInst()->GenerateItem("MpPortion");
    dummy->SetOwner(mScene->GetPlayerObj());
    if (!PushItem(dummy))
    {
        SAFE_DELETE(dummy);
    }
}

void CD2Inventory::Update(float deltaTime)
{
}

void CD2Inventory::PostUpdate(float deltaTime)
{
}

void CD2Inventory::PrevRender()
{
}

void CD2Inventory::Render()
{
}

void CD2Inventory::PostRender()
{
}

CD2Inventory* CD2Inventory::Clone()
{
	return new CD2Inventory(*this);
}

bool CD2Inventory::GrabItem(CD2Item* item)
{
    if (mGrabItem)
    {
        return false;
    }

    mGrabItem = item;
}

bool CD2Inventory::GrabItem(const int idx)
{
	// �̹� ��� �ִ� �������� ���� ���
	if (mGrabItem)
	{
		return false;
	}

	if (!isValidInvenIdx(idx))
	{
		return false;
	}

	// �ش� ���Կ� ������ ���� ���
	if (!mInvenSlot[idx].Item)
	{
		return false;
	}

	CD2Item* grabItem = mInvenSlot[idx].Item;

	mGrabItem = grabItem;

	removeItemFromSlot(idx);

	// ���� �κ��丮 ��Ͽ��� ������ ����.
	mInventory.remove(grabItem);

	return true;
}

bool CD2Inventory::UnGrabItem(const int idx)
{
	if (!mGrabItem)
	{
		return false;
	}

	CD2Item* overlapItem = nullptr;
	int overlapItemSlotIndex = -1;

	int idxX = idx % INVEN_COUNTX;
	int idxY = idx / INVEN_COUNTX;

	int grabItemWidth = mGrabItem->GetCellWidth();
	int grabItemHeight = mGrabItem->GetCellHeight();

	// ���� �������� ���� ���� ���� ���� Ž���Ͽ�, 1�� �̻��� �������� ������ ��� Ungrab���� ����.
	for (int y = 0; y < grabItemHeight; ++y)
	{
		for (int x = 0; x < grabItemWidth; ++x)
		{
			int curIndex = ((idxY + y) * INVEN_COUNTX) + idxX + x;
			CD2Item* slotItem = mInvenSlot[curIndex].Item;

			// ���� ���Կ� �������� �ִ� ���
			if (slotItem)
			{
				if (overlapItem)
				{
					// ���� Ž�� ��, 2�� �̻��� �������� ������ �����ϴ� ��� Ungrab���� ����.
					if (overlapItem != slotItem)
					{
						return false;
					}
				}
				else
				{
					// �� �������� ���� Ž������ ó�� �˻��� �������� ���
					overlapItem = slotItem;
					overlapItemSlotIndex = curIndex;
				}
			}
		}
	}

	// ���� ������ ��ġ�� �������� ���� ���, GrabItem�� ��ģ ���������� �����ϰ�,
	// ���� ��� �ִ� �������� �Էµ� �ε��� ��ġ�� ���´�.
	if (overlapItem)
	{
		CSharedPtr<CD2Item> temp = mGrabItem;
		mGrabItem = overlapItem;

		// overlapItem�� �����ϰ� �ִ� ���� ���� �����, ���� ��� �ִ� ������ ������ ���Կ� ����.
		removeItemFromSlot(overlapItemSlotIndex);
        removeItemFromInventoryList(overlapItem);

		// �κ��丮 ����Ʈ�� ��ġ�� �°� �ִ´�.
		pushItemToSlot(temp, idx);
		pushItemToInventoryList(temp, idx);
	}
	// ��ġ�� �������� ���� ���
	else
	{
		pushItemToSlot(mGrabItem, idx);
		pushItemToInventoryList(mGrabItem, idx);
        mGrabItem = nullptr;
	}

	return true;
}

bool CD2Inventory::PushItem(CD2Item* item)
{
	// ��� �������̰�, �ش��ϴ� ���ĭ�� ����ִ� ���¶��, ���ĭ�� �ִ´�.
	eD2ItemType type = item->GetItemType();

	if ((int)type < (int)eD2ItemType::Etc)
	{
        if (mEquipment[(int)type] == nullptr)
        {
			mEquipment[(int)type] = item;
			static_cast<CD2EquipItem*>(item)->Equip();
            return true;
        }
    }

    int width = item->GetCellWidth();
    int height = item->GetCellHeight();

    int idx = -1;

	bool bExistSpace = true;

    // �κ��丮 ������ Ž���ؼ�, �� �� �ִ� ������ �ִ��� ã�´�.
    for (int i = 0; i < INVEN_COUNTX * INVEN_COUNTY; ++i)
    {
        if (mInvenSlot[i].Item == nullptr)
        {
            int curIdxX = i % INVEN_COUNTX;
            int curIdxY = i / INVEN_COUNTX;

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // ������ ũ�� ���� ���� �ٸ� �������� �����ϴ� ���, ���� �ε����� �� �� ����.
                    if (mInvenSlot[(y + curIdxY) * INVEN_COUNTX + curIdxX + x].Item)
                    {
                        bExistSpace = false;
                        break;
                    }
                }
                if (!bExistSpace)
                {
                    break;
                }
            }

            if (bExistSpace)
            {
                idx = i;
                break;
            }
        }
    }

    // �� ������ ���� ���
    if (!bExistSpace)
    {
        return false;
    }

    pushItemToSlot(item, idx);
    pushItemToInventoryList(item, idx);
    
    return true;
}

bool CD2Inventory::DropItem()
{
    if (!mGrabItem)
    {
        return false;
    }
	// ������Ʈ ���� �� Ÿ���� Ž���Ѵ�.
	Vector3 ownerPos = mObject->GetWorldPos();
	Vector3 aroundPos = mObject->GetScene()->GetNavigationManager()->GetAroundEmptyTile(ownerPos);

	if (aroundPos == Vector3::Zero)
	{
		return false;
	}

	CGameObject* itemObj = CD2ObjectPool::GetInst()->ActiveInteractionObject("ItemObjTemplate", aroundPos);
	itemObj->Enable(true);
	itemObj->Start();
	static_cast<CD2ItemObjectComponent*>(itemObj->FindObjectComponentFromType<CD2ItemObjectComponent>())->SetItem(mGrabItem);

    mGrabItem = nullptr;

    return true;
}

bool CD2Inventory::UseItem(const int idx)
{
    if (!isValidInvenIdx(idx))
    {
        return false;
    }

    if (mGrabItem)
    {
        return false;
    }

    CD2Item* item = mInvenSlot[idx].Item;

    if (eD2ItemUseType::Usable != item->GetItemUseType())
    {
        return false;
    }

    static_cast<CD2UsableItem*>(item)->UseItem();

    removeItemFromInventoryList(item);
    removeItemFromSlot(idx);

    return true;
}

bool CD2Inventory::EquipItem(const int idx)
{
    if (!isValidInvenIdx(idx))
    {
        return false;
    }

    if (mGrabItem)
    {
        return false;
    }

    CD2Item* item = mInvenSlot[idx].Item;
    
    if (eD2ItemUseType::Equip != item->GetItemUseType())
    {
        return false;
    }

    eD2ItemType type = item->GetItemType();

    if (!isValidEquipmentEnum(type))
    {
        return false;
    }

    // ���� ���ĭ�� ��� ���� ���, ���� ����Ϸ��� �����۰� ��ġ ��ü���ش�.
    if (mEquipment[(int)type])
    {
        static_cast<CD2EquipItem*>(mEquipment[(int)type].Get())->UnEquip();

        int toEquipRootIndex = mInvenSlot[idx].RootSlotIndex;

        // ���Կ� ���� ����, �κ��丮 ����Ʈ�� push
        pushItemToSlot(mEquipment[(int)type], toEquipRootIndex);
        pushItemToInventoryList(mEquipment[(int)type], toEquipRootIndex);
    }
    else 
    {
        // ������ ����.
        removeItemFromSlot(idx);
    }

    mEquipment[(int)type] = item;
    static_cast<CD2EquipItem*>(item)->Equip();

    // ����Ϸ��� ������ �κ��丮 ����Ʈ���� ����
    removeItemFromInventoryList(item);

    return true;
}

bool CD2Inventory::UnEquipItem(eD2ItemType eType)
{
    if (mGrabItem)
    {
        return false;
    }

    if (!mEquipment[(int)eType])
    {
        return false;
    }

    if (!PushItem(mEquipment[(int)eType]))
    {
        return false;
    }
    
    static_cast<CD2EquipItem*>(mEquipment[(int)eType].Get())->UnEquip();
    mEquipment[(int)eType] = nullptr;

    return true;
}

bool CD2Inventory::GrabEquipItem(eD2ItemType eType)
{
    if (mGrabItem)
    {
        return false;
    }

    if (!mEquipment[(int)eType])
    {
        return false;
    }

    mGrabItem = mEquipment[(int)eType];
    mEquipment[(int)eType] = nullptr;
    static_cast<CD2EquipItem*>(mGrabItem.Get())->UnEquip();
    
    return true;
}

bool CD2Inventory::UnGrabEquipItem(eD2ItemType eType)
{
    if (!mGrabItem)
    {
        return false;
    }

    eD2ItemType grabItemType = mGrabItem->GetItemType();

    if (!isValidEquipmentEnum(grabItemType))
    {
        return false;
    }

    // ���� ��� �ִ� ����, ����Ϸ��� ĭ�� ��� Ÿ���� �ٸ��ٸ� �������� ����
    if (grabItemType != eType)
    {
        return false;
    }

    CSharedPtr<CD2Item> prevEquipItem = mEquipment[(int)grabItemType];

    mEquipment[(int)grabItemType] = mGrabItem;
    static_cast<CD2EquipItem*>(mGrabItem.Get())->Equip();

    mGrabItem = prevEquipItem;

    return true;
}

bool CD2Inventory::GrabQuickSlotItem(const int idx)
{
    if (mGrabItem)
    {
        return false;
    }

    if (!isValidQuickSlotIdx(idx))
    {
        return false;
    }

    mGrabItem = mQuickSlot[idx];
    mQuickSlot[idx] = nullptr;

    return true;
}

bool CD2Inventory::UnGrabQuickSlotItem(const int idx)
{
    if (!mGrabItem)
    {
        return false;
    }

    if (!isValidQuickSlotIdx(idx))
    {
        return false;
    }

    eD2ItemUseType useType = mGrabItem->GetItemUseType();

    if (useType != eD2ItemUseType::Usable)
    {
        return false;
    }

    CD2Item* quickSlotItem = nullptr;
    quickSlotItem = mQuickSlot[idx];

    mQuickSlot[idx] = mGrabItem;
    mGrabItem = quickSlotItem;

    return true;
}

bool CD2Inventory::UseQuickSlotItem(const int idx)
{
    if (!isValidQuickSlotIdx(idx))
    {
        return false;
    }

    if (!mQuickSlot[idx])
    {
        return false;
    }

    static_cast<CD2UsableItem*>(mQuickSlot[idx].Get())->UseItem();
    mQuickSlot[idx] = nullptr;

    return true;
}

CD2Item* CD2Inventory::GetEquipItem(eD2ItemType type)
{
    if (!isValidEquipmentEnum(type))
    {
        return nullptr;
    }

    return mEquipment[(int)type];
}

CD2Item* CD2Inventory::GetOverlappedItem(const int idx)
{
    if (!isValidInvenIdx(idx))
    {
        return nullptr;
    }

    if (!mGrabItem)
    {
        return nullptr;
    }

    int idxX = idx % INVEN_COUNTX;
    int idxY = idx / INVEN_COUNTX;

    int rangeX = mGrabItem->GetCellWidth();
    int rangeY = mGrabItem->GetCellHeight();

    for (int y = 0; y < rangeY; ++y)
    {
        for (int x = 0; x < rangeX; ++x)
        {
            if (mInvenSlot[(idxY + y) * INVEN_COUNTX + (idxX + x)].Item)
            {
                return mInvenSlot[(idxY + y) * INVEN_COUNTX + (idxX + x)].Item;
            }
        }
    }

    return nullptr;
}

int CD2Inventory::GetOverlappedItemRootIndex(const int idx)
{
	if (!isValidInvenIdx(idx))
	{
		return -1;
	}

    if (!mGrabItem)
    {
        return -1;
    }

    int idxX = idx % INVEN_COUNTX;
    int idxY = idx / INVEN_COUNTX;

    int rangeX = mGrabItem->GetCellWidth();
    int rangeY = mGrabItem->GetCellHeight();

    for (int y = 0; y < rangeY; ++y)
    {
        for (int x = 0; x < rangeX; ++x)
        {
            if (mInvenSlot[(idxY + y) * INVEN_COUNTX + (idxX + x)].Item)
            {
                return mInvenSlot[(idxY + y) * INVEN_COUNTX + (idxX + x)].RootSlotIndex;
            }
        }
    }

    return -1;
}

int CD2Inventory::GetRootSlotIndex(const int idx)
{
    if (!isValidInvenIdx(idx))
    {
        return -1;
    }

    if (!mInvenSlot[idx].Item)
    {
        return -1;
    }

    return mInvenSlot[idx].RootSlotIndex;
}

bool CD2Inventory::GetItemOccupiedIndexes(const int idx, std::vector<int>& outVec)
{
    if (!isValidInvenIdx(idx))
    {
        return false;
    }

    if (!mInvenSlot[idx].Item)
    {
        return false;
    }

    outVec = mInvenSlot[idx].VecSlotIndex;
    return true;
}

bool CD2Inventory::IsRightEquipmentSlot(eD2ItemType type)
{
    if (!mGrabItem)
    {
        return true;
    }

    eD2ItemType grabItemType = mGrabItem->GetItemType();

    if (!isValidEquipmentEnum(grabItemType))
    {
        return false;
    }

    if (type != grabItemType)
    {
        return false;
    }

    return true;
}

void CD2Inventory::GetEquipmentSlotState(std::vector<class CD2Item*>& outVecItem)
{
    outVecItem.resize((int)eD2ItemType::Etc);

    for (int i = 0; i < (int)eD2ItemType::Etc; ++i)
    {
        outVecItem[i] = mEquipment[i];
    }
}

void CD2Inventory::GetInventorySlotState(std::vector<D2ItemSlotInfo>& outVecSlotInfo)
{
    for (int i = 0; i < INVEN_COUNTX * INVEN_COUNTY; ++i)
    {
        if (mInvenSlot[i].bRootSlot)
        {
            D2ItemSlotInfo info;
            info.Item = mInvenSlot[i].Item;
            info.SlotRootIndex = i;
            outVecSlotInfo.push_back(info);
        }
    }
}

void CD2Inventory::GetQuickSlotState(std::vector<class CD2Item*>& outVecItem)
{
    outVecItem.resize(4);
    for (int i = 0; i < 4; ++i)
    {
        outVecItem[i] = mQuickSlot[i];
    }
}

bool CD2Inventory::isValidInvenIdx(const int idx)
{
    if (0 > idx || idx >= INVEN_COUNTX * INVEN_COUNTY)
    {
        return false;
    }
    return true;
}

bool CD2Inventory::isValidQuickSlotIdx(const int idx)
{
    if (0 > idx || idx >= 4)
    {
        return false;
    }
    return true;
}

bool CD2Inventory::isValidEquipmentEnum(eD2ItemType type)
{
    if ((int)type > (int)(eD2ItemType::Boots))
    {
        return false;
    }
    return true;
}

void CD2Inventory::removeItemFromSlot(const int idx)
{
    size_t size = mInvenSlot[idx].VecSlotIndex.size();

    for (size_t i = 0; i < size; ++i)
    {
        int index = mInvenSlot[idx].VecSlotIndex[i];
        mInvenSlot[index].bRootSlot = false;
        mInvenSlot[index].RootSlotIndex = -1;
        mInvenSlot[index].Item = nullptr;

        if (idx != index)
        {
			mInvenSlot[index].VecSlotIndex.clear();
        }
    }
    mInvenSlot[idx].VecSlotIndex.clear();
}

void CD2Inventory::pushItemToSlot(CD2Item* item, const int idx)
{
    mInvenSlot[idx].bRootSlot = true;
    mInvenSlot[idx].RootSlotIndex = idx;

    int width = item->GetCellWidth();
    int height = item->GetCellHeight();

    int idxX = idx % INVEN_COUNTX;
    int idxY = idx / INVEN_COUNTX;

    std::vector<int> vecIdx;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            mInvenSlot[(idxY + y) * INVEN_COUNTX + idxX + x].Item = item;
            vecIdx.push_back((idxY + y) * INVEN_COUNTX + idxX + x);
        }
    }

    size_t size = vecIdx.size();    

    for (size_t i = 0; i < size; ++i)
    {
        mInvenSlot[vecIdx[i]].VecSlotIndex = vecIdx;
        mInvenSlot[vecIdx[i]].RootSlotIndex = idx;
    }
}

int CD2Inventory::getNextItemIndex(const int idx)
{
    int result = 0;
    bool bNextItemExist = false;

	auto iter = mInventory.begin();
	auto iterEnd = mInventory.end();
	CD2Item* nextItem;

    for (int i = idx; i < INVEN_COUNTX * INVEN_COUNTY; ++i)
    {
        // �Էµ� �ε��� �ڿ� �������� ������ ���
        if (mInvenSlot[i].bRootSlot)
        {
            nextItem = mInvenSlot[i].Item;

            for (; iter != iterEnd;)
            {
                if (*iter == nextItem)
                {
                    bNextItemExist = true;
                    break;
                }

                ++result;
                std::advance(iter, 1);
            }
        }

        if (bNextItemExist)
        {
            break;
        }
    }

    if (!bNextItemExist)
    {
        return -1;
    }

    return result;
}

void CD2Inventory::pushItemToInventoryList(CD2Item* item, const int idx)
{
    int nextItemIdx = getNextItemIndex(idx);

    if (-1 == nextItemIdx)
    {
        mInventory.push_back(item);
    }
    else
    {
		auto iter = mInventory.begin();
		std::advance(iter, nextItemIdx);
		mInventory.insert(iter, item);
    }
}

void CD2Inventory::removeItemFromInventoryList(CD2Item* item)
{
    mInventory.remove(item);
}

