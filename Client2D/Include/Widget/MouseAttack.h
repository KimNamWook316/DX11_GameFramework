#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CMouseAttack :
    public CWidgetWindow
{
    friend class CViewport;
    friend class CWidgetWindow;
    friend class CEngine;

protected:
    CMouseAttack();
    CMouseAttack(const CMouseAttack& window);
    virtual ~CMouseAttack();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CMouseAttack* Clone() override;

private:
    CSharedPtr<CImage> mImage;
};

