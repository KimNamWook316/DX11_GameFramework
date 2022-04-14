#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/ProgressBar.h"
#include "Widget/Text.h"

class CD2MonsterStatusWindow :
    public CWidgetWindow
{
    friend class CViewport;

protected:
    CD2MonsterStatusWindow();
    CD2MonsterStatusWindow(const CD2MonsterStatusWindow& window);
    virtual ~CD2MonsterStatusWindow();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CD2MonsterStatusWindow* Clone();

public:
    void MakeWidget(class CExcelData* data);

public:
    void UpdateState(const std::string& monsterName, const float percent);

private:
    CSharedPtr<CImage> mHealthBarBase;
    CSharedPtr<CProgressBar> mHealthBar;
    CSharedPtr<CText> mNameText;
};

