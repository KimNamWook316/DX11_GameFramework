#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Text.h"

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

private:
    CSharedPtr<CText> mFPSText;
    CSharedPtr<CText> mPlayerSkill;

    CSharedPtr<class CD2PlayerSkillComponent> mSkill;
};

