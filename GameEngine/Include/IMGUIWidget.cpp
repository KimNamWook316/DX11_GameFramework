#include "IMGUIWidget.h"

unsigned long CIMGUIWidget::mWidgetID = 0;

CIMGUIWidget::CIMGUIWidget()	:
	mFont(nullptr),
	mColor(1.f,1.f,1.f,1.f),
	mSize(100.f, 100.f)
{
	++mWidgetID;
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
