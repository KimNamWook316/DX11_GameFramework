#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CD2MenuBorder :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2MenuBorder();
    CD2MenuBorder(const CD2MenuBorder& window);
    virtual ~CD2MenuBorder();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2MenuBorder* Clone();

public:
    void MakeWidget(class CExcelData* data);

private:
    CSharedPtr<CImage> mBorderImage;
};

