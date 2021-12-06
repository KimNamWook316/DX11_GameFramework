#include "IMGUIWidget.h"

CIMGUIWidget::CIMGUIWidget()	:
	mFont(nullptr),
	mColor(1.f,1.f,1.f,1.f),
	mSize(100.f, 100.f)
{
}

CIMGUIWidget::~CIMGUIWidget()
{
}

bool CIMGUIWidget::Init()
{
	return true;
}

void CIMGUIWidget::Render()
{
}
