#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"

class CMainWidget :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CMainWidget();
    virtual ~CMainWidget();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;

private:
    void onStartButtonClicked();

private:
    CSharedPtr<CButton> mButton;
};

