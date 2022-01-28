#include "ComponentControlWidget.h"

CComponentControlWidget::CComponentControlWidget()	:
	mComponent(nullptr)
{
}

CComponentControlWidget::~CComponentControlWidget()
{
}

bool CComponentControlWidget::Init()
{
	CIMGUIWidgetList::Init();
	return true;
}

void CComponentControlWidget::Render()
{
	CIMGUIWidgetList::Render();
}
