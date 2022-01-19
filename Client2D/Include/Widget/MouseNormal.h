#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CMouseNormal :
    public CWidgetWindow
{
    friend class CViewport;
    friend class CWidgetWindow;

protected:
    CMouseNormal();
    CMouseNormal(const CMouseNormal& window);
    virtual ~CMouseNormal();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CMouseNormal* Clone() override;

private:
    CSharedPtr<CImage> mImage;
};

