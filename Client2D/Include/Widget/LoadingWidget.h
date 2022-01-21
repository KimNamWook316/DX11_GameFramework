#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/ProgressBar.h"
#include "Widget/Image.h"

class CLoadingWidget :
    public CWidgetWindow
{
    friend class CViewport;

public:
    CLoadingWidget();
    CLoadingWidget(const CLoadingWidget& window);
    ~CLoadingWidget();
    
public:
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CLoadingWidget* Clone();

public:
    void SetLoadingPercent(const float percent)
    {
        mLoadingBar->SetPercent(percent);
    }

private:
    CSharedPtr<CImage> mBack;
    CSharedPtr<CProgressBar> mLoadingBar;
};

