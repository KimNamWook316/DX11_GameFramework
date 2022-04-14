#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"
#include "Widget/Text.h"

class CD2IntroWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2IntroWindow();
    CD2IntroWindow(const CD2IntroWindow& window);
    virtual ~CD2IntroWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2IntroWindow* Clone();

public:
    void MakeWidget(class CExcelData* data);

private:
    void onClickStart();
    void onClickEnd();

private:
    CSharedPtr<CImage> mBackground;
    CSharedPtr<CImage> mLogo;
    CSharedPtr<CButton> mStartButton;
    CSharedPtr<CText> mStartText;
    CSharedPtr<CButton> mExitButton;
    CSharedPtr<CText> mExitText;
    CSharedPtr<class CGameObject> mParticle;
};

