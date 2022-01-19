#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Text.h"
#include "Widget/ProgressBar.h"
#include "Widget/Image.h"

class CSimpleHUD :
    public CWidgetWindow
{
    friend class CViewport;
    friend class CWidgetComponent;

protected:
    CSimpleHUD();
    CSimpleHUD(const CSimpleHUD& window);
    virtual ~CSimpleHUD();

public:
    void SetText(const TCHAR* text);
    void SetHPPercent(const float percent);
    void SetHPDir(eProgressBarDir dir);

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CSimpleHUD* Clone();

private:
    CSharedPtr<CText> mNameText;
    CSharedPtr<CProgressBar> mHPBar;
    CSharedPtr<CImage> mImage;
};

