#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"

class CStartWidget :
    public CWidgetWindow
{
    friend class CViewport;

public:
    CStartWidget();
    CStartWidget(const CStartWidget& window);
    ~CStartWidget();
    
public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CStartWidget* Clone();

public:
    void OnClickStart();
    void OnClickExit();

private:
    CSharedPtr<CButton> mStartButton;
    CSharedPtr<CButton> mExitButton;
    CSharedPtr<CImage> mBack;
};

