#include "IMGUITestWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"

CIMGUITestWindow::CIMGUITestWindow()
{
}

CIMGUITestWindow::~CIMGUITestWindow()
{
}

bool CIMGUITestWindow::Init()
{
	CIMGUIWindow::Init();
	
	CIMGUIButton* button = AddWidget<CIMGUIButton>("Button1");

	button->SetClickCallBack<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton1);
	
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(200.f);
	line->SetSpacing(100.f);

	button = AddWidget<CIMGUIButton>("Button2");

	button->SetClickCallBack<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton2);

	CIMGUILabel* label = AddWidget<CIMGUILabel>("Test", 100.f, 30.f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.f);

	line = AddWidget<CIMGUISameLine>("Line");

	CIMGUIText* text = AddWidget<CIMGUIText>("Text", 100.f, 30.f);
	text->SetText("Some Text");
	text->SetColor(0, 255, 0);

	CIMGUITextInput* textInput = AddWidget<CIMGUITextInput>("TextInput", 200.f, 30.f);
	textInput->SetHintText("Hint Text");

	return true;
}

void CIMGUITestWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CIMGUITestWindow::ClickButton1()
{
	MessageBox(0, TEXT("1"), TEXT("1"), MB_OK);
}

void CIMGUITestWindow::ClickButton2()
{
	MessageBox(0, TEXT("2"), TEXT("2"), MB_OK);
}
