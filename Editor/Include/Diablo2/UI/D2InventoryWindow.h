#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Grid.h"
#include "Widget/Image.h"
#include "../D2Info.h"

class CD2InventoryWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2InventoryWindow();
    CD2InventoryWindow(const CD2InventoryWindow& window);
    virtual ~CD2InventoryWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2InventoryWindow* Clone();

public:
    void MakeWidget(class CExcelData* data);

private:
    void SetEquipmentTexture(eD2ItemType type, class CD2Item* item);
    void SetItemSlotTexture(const int rootIdx, class CD2Item* item);

private:
    void onClickClose();
    void onClickItemSlot(int idx);
    void onClickHeadSlot();
    void onClickArmorSlot();
    void onClickMainWeaponSlot();
    void onClickSubWeaponSlot();
    void onClickBeltSlot();
    void onClickGlobeSlot();
    void onClickBootsSlot();

    void onHoverHeadSlot();
    void onHoverArmorSlot();
    void onHoverMainWeaponSlot();
    void onHoverSubWeaponSlot();
    void onHoverBeltSlot();
    void onHoverGlobeSlot();
    void onHoverBootsSlot();

    void clickEquipmentSlot(eD2ItemType type);
    void hoverEquipmentSlot(eD2ItemType type);

private:
    class CD2Inventory* mInven;
    CSharedPtr<CImage> mInvenPanel;
    CSharedPtr<CButton> mEqipmentSlot[(int)eD2ItemType::Etc];
    CSharedPtr<CGrid> mItemSlot;
    CSharedPtr<CButton> mCloseButton;
    std::vector<CSharedPtr<CImage>> mVecItemSlotImage;
};

