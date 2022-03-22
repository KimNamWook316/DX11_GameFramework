#include "D2MouseNormal.h"

CD2MouseNormal::CD2MouseNormal()
{
	SetTypeID<CD2MouseNormal>();
}

CD2MouseNormal::CD2MouseNormal(const CD2MouseNormal& window)	:
	CWidgetWindow(window)
{
	mImage = FindWidget<CImage>("D2MouseNormal");
}

CD2MouseNormal::~CD2MouseNormal()
{
}

bool CD2MouseNormal::Init()
{
	CWidgetWindow::Init();

	SetSize(32.f, 28.f);

	mImage = CreateWidget<CImage>("D2MouseNormal");

	mImage->SetTexture("MouseNormal", TEXT("Dia2UI/cursor.png"));
	mImage->SetSize(32.f, 28.f);
	for (size_t i = 0; i < 8; ++i)
	{
		mImage->AddFrameData(Vector2(i * 32.f, 0.f), Vector2(32.f, 28.f));
	}

	return true;
}

void CD2MouseNormal::Start()
{
	CWidgetWindow::Start();

}

void CD2MouseNormal::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2MouseNormal::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2MouseNormal::Render()
{
	CWidgetWindow::Render();
}

CD2MouseNormal* CD2MouseNormal::Clone()
{
	return new CD2MouseNormal(*this);
}
