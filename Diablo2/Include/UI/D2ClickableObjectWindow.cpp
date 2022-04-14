#include "D2ClickableObjectWindow.h"

CD2ClickableObjectWindow::CD2ClickableObjectWindow()
{
	SetTypeID<CD2ClickableObjectWindow>();
}

CD2ClickableObjectWindow::CD2ClickableObjectWindow(const CD2ClickableObjectWindow& window)	:
	CWidgetWindow(window)
{
	mBackGround = FindWidget<CImage>("ClickableUIBack");
	mText = FindWidget<CText>("ClickableUIText");
}

CD2ClickableObjectWindow::~CD2ClickableObjectWindow()
{
}

bool CD2ClickableObjectWindow::Init()
{
	CWidgetWindow::Init();
	
	mBackGround = CreateWidget<CImage>("ClickableUIBack");
	mText = CreateWidget<CText>("ClickableUIText");

	mBackGround->SetPos(0.f, 0.f);
	mBackGround->SetSize(40.f, 20.f);
	mText->SetPos(0.f, 0.f);
	mText->SetSize(40.f, 20.f);

	mBackGround->SetTextureTint(Vector4(0.f, 0.f, 0.f, 1.f));
	mBackGround->SetOpacity(0.7f);
	mText->SetFontSize(10.f);
	mText->SetAlignH(eTextAlignH::Center);
	mText->SetAlignV(eTextAlignV::Middle);
	mText->SetColor(1.f, 1.f, 1.f);
	mText->SetText(TEXT("Empty"));

	return true;
}

void CD2ClickableObjectWindow::Start()
{
	CWidgetWindow::Start();
}

void CD2ClickableObjectWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2ClickableObjectWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2ClickableObjectWindow::Render()
{
	CWidgetWindow::Render();
}

CD2ClickableObjectWindow* CD2ClickableObjectWindow::Clone()
{
	return new CD2ClickableObjectWindow(*this);
}

void CD2ClickableObjectWindow::SetText(const std::string& text)
{
	char buf[128] = {};
	strcpy_s(buf, text.c_str());
	TCHAR bufConvert[128] = {};
	int length = MultiByteToWideChar(CP_ACP, 0, buf, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, buf, length, bufConvert, length);

	mText->SetText(bufConvert);
	mText->SetSize(length * 8.f, 20.f);
	mBackGround->SetSize(length * 8.f, 20.f);
}
