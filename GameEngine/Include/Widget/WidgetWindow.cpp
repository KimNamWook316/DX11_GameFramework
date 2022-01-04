#include "WidgetWindow.h"

CWidgetWindow::CWidgetWindow() :
	mViewport(nullptr),
	mZOrder(0)
{
}

CWidgetWindow::~CWidgetWindow()
{
}

bool CWidgetWindow::Init()
{
	return true;
}

void CWidgetWindow::Start()
{
}

void CWidgetWindow::Update(float deltaTime)
{
}

void CWidgetWindow::PostUpdate(float deltaTime)
{
}

void CWidgetWindow::Render()
{
}
