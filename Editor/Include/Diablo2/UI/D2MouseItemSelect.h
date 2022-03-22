#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CD2MouseItemSelect :
    public CWidgetWindow
{
    friend class CViewport;
    friend class CWidgetWindow;
    friend class CEngine;

protected:
    CD2MouseItemSelect();
    CD2MouseItemSelect(const CD2MouseItemSelect& window);
    virtual ~CD2MouseItemSelect();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2MouseItemSelect* Clone() override;

public:
    void SetMouseTexture(CTexture* texture);

private:
    CSharedPtr<CImage> mImage;
};

