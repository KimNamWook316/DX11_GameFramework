#include "ComponentControlWidget.h"
#include "Component/Component.h"
#include "IMGUIText.h"
#include "IMGUISeperator.h"

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

	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	std::string label = mComponent->GetName() + " Component";
	text->SetText(label.c_str());
	CIMGUISeperator* sep = AddWidget<CIMGUISeperator>("sep");

	return true;
}

void CComponentControlWidget::Render()
{
	CIMGUIWidgetList::Render();
}
