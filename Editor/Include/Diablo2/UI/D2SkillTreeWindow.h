#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"
#include "Widget/Text.h"
#include "../D2Info.h"

struct SkillButton
{
    CSharedPtr<CButton> Button;
    CSharedPtr<CText> LevelText;
};

class CD2SkillTreeWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2SkillTreeWindow();
    CD2SkillTreeWindow(const CD2SkillTreeWindow& window);
    virtual ~CD2SkillTreeWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2SkillTreeWindow* Clone();

public:
    void MakeWidget(class CExcelData* data);

private:
    void onInit();
    void setButton(const D2SkillStateInfo& info, const SkillButton& button);
    void onChangeSkillPoint();
    void onStateChange(D2SkillStateInfo info);

private:
    void onClickSkillTree1Button();
    void onClickSkillTree2Button();
    void onClickSkillTree3Button();
    void onClickFireBolt();
    void onClickFireBall();
    void onClickBlaze();
    void onClickMeteor();
    void onClickIceBolt();
    void onClickFrozenOrb();
    void onClickTeleport();
    void onClickExit();

private:
    class CD2PlayerSkillComponent* mSkill;
    class CD2CharacterInfoComponent* mCharInfo;

    CSharedPtr<CImage> mSkillPointPanel;
    CSharedPtr<CText> mSkillPointLabelText;
    CSharedPtr<CText> mSkillPointText;

    // Tree1
    CSharedPtr<CImage> mSkillTree1Panel;
    CSharedPtr<CButton> mSkillTree1Button;
    CSharedPtr<CText> mSkillTree1Text;
    SkillButton mFireBoltButton;
    SkillButton mFireBallButton;
    SkillButton mBlazeButton;
    SkillButton mMeteorButton;
    CSharedPtr<CButton> mSkillTree1ExitButton;
    
    // Tree2
    CSharedPtr<CImage> mSkillTree2Panel;
    CSharedPtr<CButton> mSkillTree2Button;
    CSharedPtr<CText> mSkillTree2Text;
    SkillButton mIceBoltButton;
    SkillButton mFrozenOrbButton;
    CSharedPtr<CButton> mSkillTree2ExitButton;

    // Tree3
    CSharedPtr<CImage> mSkillTree3Panel;
    CSharedPtr<CText> mSkillTree3Text;
    CSharedPtr<CButton> mSkillTree3Button;
    SkillButton mTeleportButton;
    CSharedPtr<CButton> mSkillTree3ExitButton;
};

