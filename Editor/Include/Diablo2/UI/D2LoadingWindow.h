#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CD2LoadingWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2LoadingWindow();
    CD2LoadingWindow(const CD2LoadingWindow& window);
    virtual ~CD2LoadingWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2LoadingWindow* Clone();

public:
    void SetLoadingPercent(const float percent);

public:
    void MakeWidget(class CExcelData* data);

private:
    CSharedPtr<CImage> mLoadingImage;
};

