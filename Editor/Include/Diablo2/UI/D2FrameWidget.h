#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Text.h"
#include "Widget/Image.h"
#include "Widget/Grid.h"

class CD2FrameWidget :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2FrameWidget();
    CD2FrameWidget(const CD2FrameWidget& window);
    virtual ~CD2FrameWidget();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2FrameWidget* Clone();

public:
    void OnClickGrid(int idx);

private:
    CSharedPtr<CText> mFPSText;
    CSharedPtr<CText> mPlayerSkill;
    CSharedPtr<CText> mHp;
    CSharedPtr<CText> mMp;
    CSharedPtr<CImage> mImage;
    CSharedPtr<CImage> mImage2;
    CSharedPtr<CGrid> mGrid;

    class CD2PlayerSkillComponent* mSkill;
    class CD2CharacterInfoComponent* mCharInfo;
};

