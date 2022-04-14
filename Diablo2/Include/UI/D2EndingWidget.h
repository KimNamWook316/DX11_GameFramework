#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CD2EndingWidget :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2EndingWidget();
    CD2EndingWidget(const CD2EndingWidget& window);
    virtual ~CD2EndingWidget();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2EndingWidget* Clone();

public:
    void MakeWidget(class CExcelData* data);

private:
    CSharedPtr<CImage> mEndingImage;
};

