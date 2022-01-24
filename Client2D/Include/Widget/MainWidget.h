#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Text.h"
#include "Widget/Number.h"

class CMainWidget :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CMainWidget();
    CMainWidget(const CMainWidget& window);
    virtual ~CMainWidget();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CMainWidget* Clone();

private:
    void onStartButtonClicked();

private:
    CSharedPtr<CButton> mButton;
    CSharedPtr<CText> mText;
    CSharedPtr<CNumber> mHour;
    CSharedPtr<CNumber> mMinute;
    CSharedPtr<CNumber> mSecond;
    CSharedPtr<CText> mFPSText;
};

