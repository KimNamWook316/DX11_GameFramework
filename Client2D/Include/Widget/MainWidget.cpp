#include "MainWidget.h"

CMainWidget::CMainWidget()
{
}

CMainWidget::CMainWidget(const CMainWidget& window)
{
	mButton = FindWidget<CButton>("Button");
	mText = FindWidget<CText>("Text");
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
	if (!CWidgetWindow::Init())
	{
		return false;
	}
	
	SetSize(1280.f, 720.f);

	mButton = CreateWidget<CButton>("Button");

	mButton->SetPos(400.f, 300.f);
	mButton->SetSize(200.f, 50.f);
	mButton->SetTexture(eButtonState::Normal, "StartButton", TEXT("Start.png"));
	mButton->SetTexture(eButtonState::Hovered, "StartButton", TEXT("Start.png"));
	mButton->SetTexture(eButtonState::Clicked, "StartButton", TEXT("Start.png"));

	mButton->SetTextureTint(eButtonState::Normal, 220, 220, 220, 255);
	mButton->SetTextureTint(eButtonState::Hovered, 255, 255, 255, 255);
	mButton->SetTextureTint(eButtonState::Clicked, 128, 128, 128, 128);

	mButton->SetClickCallBack(this, &CMainWidget::onStartButtonClicked);
	
	mButton->SetSound(eButtonSoundState::Hovered, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
	mButton->SetSound(eButtonSoundState::Clicked, "UI", "ButtonClick", "TeemoStartClicck.mp3");

	mText = CreateWidget<CText>("Text");
	mText->SetText(TEXT("Hello"));
	mText->SetPos(200.f, 400.f);
	mText->SetSize(100.f, 40.f);
	mText->SetZorder(1);
	mText->SetAlignH(eTextAlignH::Center);
	mText->SetAlignV(eTextAlignV::Middle);
	mText->SetShadowEnable(true);
	mText->SetShadowColor(0.f, 1.f, 0.f);
	mText->SetShadowOffset(2.f, 2.f);
	return true;
}

void CMainWidget::Start()
{
	CWidgetWindow::Start();
}

void CMainWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CMainWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMainWidget::Render()
{
	CWidgetWindow::Render();
}

CMainWidget* CMainWidget::Clone()
{
	return new CMainWidget(*this);
}

void CMainWidget::onStartButtonClicked()
{
	//MessageBox(nullptr, TEXT("Click"), TEXT("Click"), MB_OK);
}
