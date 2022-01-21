#include "LoadingWidget.h"
#include "Engine.h"
#include "../Scene/MainScene.h"
#include "Scene/SceneManager.h"

CLoadingWidget::CLoadingWidget()
{
}

CLoadingWidget::CLoadingWidget(const CLoadingWidget& window)    :
    CWidgetWindow(window)
{
    mBack = FindWidget<CImage>("Back");
    mLoadingBar = FindWidget<CProgressBar>("LoadingBar");
}

CLoadingWidget::~CLoadingWidget()
{
}

bool CLoadingWidget::Init()
{
    if (!CWidgetWindow::Init())
    {
        assert(false);
        return false;
    }

    SetSize(1280.f, 720.f);

    mBack = CreateWidget<CImage>("Back");
    mLoadingBar = CreateWidget<CProgressBar>("LoadingBar");

	mBack->SetTexture("LoadingBack", TEXT("LoadingBack.jpg"));
	mBack->SetSize(1280.f, 720.f);

    mLoadingBar->SetPos(150.f, 100.f);
    mLoadingBar->SetSize(980.f, 40.f);
    mLoadingBar->SetTexture("LoadingBar", TEXT("HPBar.png"));
    mLoadingBar->SetZorder(1);
    mLoadingBar->SetPercent(0.f);

    return true;
}

void CLoadingWidget::Update(float deltaTime)
{
    CWidgetWindow::Update(deltaTime);
}

void CLoadingWidget::PostUpdate(float deltaTime)
{
    CWidgetWindow::PostUpdate(deltaTime);
}

void CLoadingWidget::Render()
{
    CWidgetWindow::Render();
}

CLoadingWidget* CLoadingWidget::Clone()
{
    return new CLoadingWidget(*this);
}
