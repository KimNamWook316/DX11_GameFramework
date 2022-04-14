#pragma once

#include "Component/ObjectComponent.h"
#include "../D2Info.h"

#define INVEN_COUNTX 10
#define INVEN_COUNTY 4

struct SlotElement
{
    std::vector<int> VecSlotIndex;
    class CD2Item* Item;
    int RootSlotIndex;
    bool bRootSlot;

    SlotElement()   :
        Item(nullptr),
        bRootSlot(false),
        RootSlotIndex(-1)
    {
    }
};

class CD2Inventory :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2Inventory();
    CD2Inventory(const CD2Inventory& com);
    virtual ~CD2Inventory();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2Inventory* Clone() override;

public:
    bool GrabItem(class CD2Item* item);
    bool GrabItem(const int idx);
    bool UnGrabItem(const int idx);
    bool PushItem(class CD2Item* item);
    bool DropItem();
    bool UseItem(const int idx);

    bool EquipItem(const int idx);
    bool UnEquipItem(eD2ItemType eType);
    bool GrabEquipItem(eD2ItemType eType);
    bool UnGrabEquipItem(eD2ItemType eType);

    bool GrabQuickSlotItem(const int idx);
    bool UnGrabQuickSlotItem(const int idx);
    bool UseQuickSlotItem(const int idx);

public:
    CD2Item* GetGrabItem()
    {
        return mGrabItem;
    }
    CD2Item* GetItem(const int idx)
    {
        return mInvenSlot[idx].Item;
    }
    CD2Item* GetQuickSlotItem(const int idx)
    {
        if (!isValidQuickSlotIdx(idx))
        {
            return nullptr;
        }

        return mQuickSlot[idx];
    }
    CD2Item* GetEquipItem(eD2ItemType type);
    CD2Item* GetOverlappedItem(const int idx);
    int GetOverlappedItemRootIndex(const int idx);
    int GetRootSlotIndex(const int idx);
    bool GetItemOccupiedIndexes(const int idx, std::vector<int>& outVec);
    bool IsRightEquipmentSlot(eD2ItemType type);

public:
    void GetEquipmentSlotState(std::vector<class CD2Item*>& outVecItem);
    void GetInventorySlotState(std::vector<D2ItemSlotInfo>& outVecSlotInfo);
    void GetQuickSlotState(std::vector<class CD2Item*>& outVecItem);

private:
    bool isValidInvenIdx(const int idx);
    bool isValidQuickSlotIdx(const int idx);
    bool isValidEquipmentEnum(eD2ItemType type);
    void pushItemToSlot(CD2Item* item, const int idx);
    void removeItemFromSlot(const int idx);
    void pushItemToInventoryList(CD2Item* item, const int idx);
    void removeItemFromInventoryList(CD2Item* item);
    int getNextItemIndex(const int idx);

private:
    CSharedPtr<CD2Item> mGrabItem;
    CSharedPtr<CD2Item> mQuickSlot[4];
    CSharedPtr<CD2Item> mEquipment[(int)eD2ItemType::Etc];
    std::list<CSharedPtr<CD2Item>> mInventory;
    SlotElement mInvenSlot[INVEN_COUNTX * INVEN_COUNTY];
};

