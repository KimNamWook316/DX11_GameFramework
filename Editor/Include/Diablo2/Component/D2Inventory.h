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
    bool GrabItem(const int idx);
    bool UnGrabItem(const int idx);
    bool PushItem(class CD2Item* item);
    void DropItem();
    bool UseItem(const int idx);

    bool EquipItem(const int idx);
    bool UnEquipItem(eD2ItemType eType);
    bool GrabEquipItem(eD2ItemType eType);
    bool UnGrabEquipItem(eD2ItemType eType);

    bool GrabQuickSlotItem(const int idx);
    bool UnGrabQuickSlotItem(const int idx);
    bool UseQuickSlotItem(const int idx);

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
    class CD2Item* mGrabItem;
    class CD2Item* mQuickSlot[4];
    class CD2Item* mEquipment[(int)eD2ItemType::Etc];
    std::list<class CD2Item*> mInventory;
    SlotElement mInvenSlot[INVEN_COUNTX * INVEN_COUNTY];
};

