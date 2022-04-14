#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CD2MouseNormal :
    public CWidgetWindow
{
    friend class CViewport;
    friend class CWidgetWindow;
    friend class CEngine;

protected:
    CD2MouseNormal();
    CD2MouseNormal(const CD2MouseNormal& window);
    virtual ~CD2MouseNormal();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2MouseNormal* Clone() override;

private:
    CSharedPtr<CImage> mImage;
};

