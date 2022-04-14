#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Text.h"
#include "../D2Info.h"

struct D2SkillSelectButton
{
    CSharedPtr<CButton> Button;
    CSharedPtr<CText> Text;
};

class CD2SkillSelectWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2SkillSelectWindow();
    CD2SkillSelectWindow(const CD2SkillSelectWindow& window);
    virtual ~CD2SkillSelectWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2SkillSelectWindow* Clone();

public:
    void AddButton(const D2SkillStateInfo& info);

private:
    void onInit();
    void setCallBack(CButton* button, const std::string& skillName);
    
private:
    void onSkillLevelUp(D2SkillStateInfo info);
    void onClickFireBolt();
    void onClickFireBall();
    void onClickBlaze();
    void onClickMeteor();
    void onClickIceBolt();
    void onClickFrozenOrb();
    void onClickTeleport();

private:
    class CD2PlayerSkillComponent* mSkill;
    std::vector<D2SkillSelectButton> mSkillSelectButton[(int)eD2SkillTreeNo::Max];
    Vector2 mButtonStartPos[(int)eD2SkillTreeNo::Max];
};

