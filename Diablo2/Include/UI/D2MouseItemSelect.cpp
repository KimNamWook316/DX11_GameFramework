#include "D2MouseItemSelect.h"
#include "Resource/Texture/Texture.h"

CD2MouseItemSelect::CD2MouseItemSelect()
{
	SetTypeID<CD2MouseItemSelect>();
}

CD2MouseItemSelect::CD2MouseItemSelect(const CD2MouseItemSelect& window)	:
	CWidgetWindow(window)
{
	mImage = FindWidget<CImage>("D2MouseItemSelect");
}

CD2MouseItemSelect::~CD2MouseItemSelect()
{
}

bool CD2MouseItemSelect::Init()
{
	CWidgetWindow::Init();

	mImage = CreateWidget<CImage>("D2MouseItemSelect");

	return true;
}

void CD2MouseItemSelect::Start()
{
	CWidgetWindow::Start();
}

void CD2MouseItemSelect::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2MouseItemSelect::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2MouseItemSelect::Render()
{
	CWidgetWindow::Render();
}

CD2MouseItemSelect* CD2MouseItemSelect::Clone()
{
	return new CD2MouseItemSelect(*this);
}

void CD2MouseItemSelect::SetMouseTexture(CTexture* texture)
{
	SetSize(texture->GetWidth(), texture->GetHeight());

	mImage->SetTexture(texture);
	mImage->SetSize(texture->GetWidth(), texture->GetHeight());
}
