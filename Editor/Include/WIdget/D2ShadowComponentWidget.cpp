#include "D2ShadowComponentWidget.h"
#include "IMGUISeperator.h"
#include "IMGUIText.h"
#include "IMGUIInputFloat2.h"
#include "../Diablo2/Component/D2ShadowComponent.h"

CD2ShadowComponentWidget::CD2ShadowComponentWidget()	:
	mOffset(nullptr)
{
}

CD2ShadowComponentWidget::~CD2ShadowComponentWidget()
{
}

bool CD2ShadowComponentWidget::Init()
{
	// Create Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("D2Shadow Component");
	mOffset = AddWidget<CIMGUIInputFloat2>("Shadow Offset");
	AddWidget<CIMGUISeperator>("sperator");

	// Initial Value
	Vector2 offset = static_cast<CD2ShadowComponent*>(mComponent.Get())->GetOffset();
	mOffset->SetVal(offset.x, offset.y);

	// CallBack
	mOffset->SetCallBack(this, &CD2ShadowComponentWidget::OnChangeOffset);
	return true;
}

void CD2ShadowComponentWidget::Render()
{
	CComponentControlWidget::Render();
}

void CD2ShadowComponentWidget::OnChangeOffset(float val[2])
{
	static_cast<CD2ShadowComponent*>(mComponent.Get())->SetOffset(Vector2(val[0], val[1]));
}
