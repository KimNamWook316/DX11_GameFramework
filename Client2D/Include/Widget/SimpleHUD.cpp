#include "SimpleHUD.h"

CSimpleHUD::CSimpleHUD()
{
}

CSimpleHUD::CSimpleHUD(const CSimpleHUD& window)	:
	CWidgetWindow(window)
{
	mNameText = FindWidget<CText>("NameText");
	mHPBar = FindWidget<CProgressBar>("HPBar");
}

CSimpleHUD::~CSimpleHUD()
{
}

void CSimpleHUD::SetText(const TCHAR* text)
{
	mNameText->SetText(text);
}

void CSimpleHUD::SetHPPercent(const float percent)
{
	mHPBar->SetPercent(percent);
}

void CSimpleHUD::SetHPDir(eProgressBarDir dir)
{
	mHPBar->SetDir(dir);
}

bool CSimpleHUD::Init()
{
	if (!CWidgetWindow::Init())
	{
		assert(false);
		return false;
	}

	SetSize(100.f, 50.f);

	mHPBar = CreateWidget<CProgressBar>("HPBar");
	mNameText = CreateWidget<CText>("NameText");

	mNameText->SetText(TEXT("Name"));
	mNameText->SetPos(25.f, 0.f);
	mNameText->SetFontSize(13.f);
	mNameText->SetZorder(1);
	mNameText->SetColor(1.f, 1.f, 1.f);
	mNameText->SetAlignH(eTextAlignH::Center);
	mNameText->SetAlignV(eTextAlignV::Top);

	mHPBar->SetSize(100.f, 20.f);
	mHPBar->SetTextureTint(255, 0, 0, 255);

	return true;
}

void CSimpleHUD::Start()
{
	CWidgetWindow::Start();
}

void CSimpleHUD::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CSimpleHUD::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CSimpleHUD::Render()
{
	CWidgetWindow::Render();
}

CSimpleHUD* CSimpleHUD::Clone()
{
	return new CSimpleHUD(*this);
}
