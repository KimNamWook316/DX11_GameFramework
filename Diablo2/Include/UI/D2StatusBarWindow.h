#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Grid.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"
#include "../D2Info.h"

class CD2StatusBarWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2StatusBarWindow();
    CD2StatusBarWindow(const CD2StatusBarWindow& window);
    virtual ~CD2StatusBarWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2StatusBarWindow* Clone();

public:
    void MakeWidget(class CExcelData* data);

public:
    void SetRSkillButtonTexture(const std::string& skillName);
    void SetQuickSlotTintEnable(bool bEnable);

public:
    void OnRemainSkillPointZero();

private:
    void onHpChange();
    void onMpChange();
    void onExpChange();
    void onLevelUp();
    void onClickRSkillButton();
    void onClickOpenSkillTree();
    void onLClickQuickSlot(int idx);
    void onRClickQuickSlot(int idx);
    void setQuickSlotTexture(const int idx, class CD2Item* item);

private:
    class CD2CharacterInfoComponent* mCharInfo;
    class CD2Inventory* mInven;
    class CD2PlayerSkillComponent* mSkill;

    CSharedPtr<CImage> mStatusBarPanel;
    CSharedPtr<CProgressBar> mHpBar;
    CSharedPtr<CProgressBar> mMpBar;
    CSharedPtr<CProgressBar> mStaminaBar;
    CSharedPtr<CProgressBar> mExpBar;
    CSharedPtr<CGrid> mQuickSlot;
    CSharedPtr<CImage> mQuickSlotImage[4];
    CSharedPtr<CButton> mLeftSkillButton;
    CSharedPtr<CButton> mRightSkillButton;
    CSharedPtr<CButton> mOpenSkillTreeButton;
};

