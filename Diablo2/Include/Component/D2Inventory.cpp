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
	// 이미 잡고 있는 아이템이 있을 경우
	if (mGrabItem)
	{
		return false;
	}

	if (!isValidInvenIdx(idx))
	{
		return false;
	}

	// 해당 슬롯에 아이템 없을 경우
	if (!mInvenSlot[idx].Item)
	{
		return false;
	}

	CD2Item* grabItem = mInvenSlot[idx].Item;

	mGrabItem = grabItem;

	removeItemFromSlot(idx);

	// 현재 인벤토리 목록에서 아이템 삭제.
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

	// 현재 아이템을 놓고 싶은 범위 안을 탐색하여, 1개 이상의 아이템이 존재할 경우 Ungrab하지 못함.
	for (int y = 0; y < grabItemHeight; ++y)
	{
		for (int x = 0; x < grabItemWidth; ++x)
		{
			int curIndex = ((idxY + y) * INVEN_COUNTX) + idxX + x;
			CD2Item* slotItem = mInvenSlot[curIndex].Item;

			// 현재 슬롯에 아이템이 있는 경우
			if (slotItem)
			{
				if (overlapItem)
				{
					// 슬롯 탐색 중, 2개 이상의 아이템이 범위에 존재하는 경우 Ungrab하지 못함.
					if (overlapItem != slotItem)
					{
						return false;
					}
				}
				else
				{
					// 이 아이템이 슬롯 탐색에서 처음 검색된 아이템일 경우
					overlapItem = slotItem;
					overlapItemSlotIndex = curIndex;
				}
			}
		}
	}

	// 현재 범위에 겹치는 아이템이 있을 경우, GrabItem을 겹친 아이템으로 변경하고,
	// 현재 들고 있는 아이템을 입력된 인덱스 위치에 놓는다.
	if (overlapItem)
	{
		CSharedPtr<CD2Item> temp = mGrabItem;
		mGrabItem = overlapItem;

		// overlapItem이 차지하고 있는 슬롯 정보 지우고, 현재 들고 있는 아이템 정보를 슬롯에 쓴다.
		removeItemFromSlot(overlapItemSlotIndex);
        removeItemFromInventoryList(overlapItem);

		// 인벤토리 리스트에 위치에 맞게 넣는다.
		pushItemToSlot(temp, idx);
		pushItemToInventoryList(temp, idx);
	}
	// 겹치는 아이템이 없는 경우
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
	// 장비 아이템이고, 해당하는 장비칸이 비어있는 상태라면, 장비칸에 넣는다.
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

    // 인벤토리 위부터 탐색해서, 들어갈 수 있는 공간이 있는지 찾는다.
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
                    // 아이템 크기 범위 내에 다른 아이템이 존재하는 경우, 지금 인덱스에 들어갈 수 없다.
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

    // 들어갈 공간이 없는 경우
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
	// 오브젝트 주위 빈 타일을 탐색한다.
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

    // 현재 장비칸에 장비가 있을 경우, 지금 장비하려는 아이템과 위치 교체해준다.
    if (mEquipment[(int)type])
    {
        static_cast<CD2EquipItem*>(mEquipment[(int)type].Get())->UnEquip();

        int toEquipRootIndex = mInvenSlot[idx].RootSlotIndex;

        // 슬롯에 정보 쓰고, 인벤토리 리스트에 push
        pushItemToSlot(mEquipment[(int)type], toEquipRootIndex);
        pushItemToInventoryList(mEquipment[(int)type], toEquipRootIndex);
    }
    else 
    {
        // 슬롯을 비운다.
        removeItemFromSlot(idx);
    }

    mEquipment[(int)type] = item;
    static_cast<CD2EquipItem*>(item)->Equip();

    // 장비하려는 아이템 인벤토리 리스트에서 제거
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

    // 현재 들고 있는 장비와, 장비하려는 칸의 장비 타입이 다르다면 장착하지 못함
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
        // 입력된 인덱스 뒤에 아이템이 존재할 경우
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

