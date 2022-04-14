#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Text.h"

class CD2ClickableObjectWindow :
    public CWidgetWindow
{
    friend class CViewport;
    friend class CWidgetComponent;

protected:
    CD2ClickableObjectWindow();
    CD2ClickableObjectWindow(const CD2ClickableObjectWindow& window);
    virtual ~CD2ClickableObjectWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2ClickableObjectWindow* Clone();

public:
    void SetText(const std::string& text);

private:
    CSharedPtr<CImage> mBackGround;
    CSharedPtr<CText> mText;
};

