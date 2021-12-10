#include "IMGUITestWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h";
#include "IMGUIListBox.h";
#include "IMGUIImage.h"
#include "IMGUICheckBox.h"
#include "IMGUIRadioButton.h"

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
	textInput->SetHideName(true);

	CIMGUIListBox* listBox = AddWidget<CIMGUIListBox>("ListBox", 200.f, 30.f);
	listBox->SetHideName(true);
	listBox->AddItem("Hello");
	listBox->AddItem("This is");
	listBox->AddItem("ListBox Widget");
	listBox->AddItem("Click Me");
	listBox->AddItem("한글출력테스트");

	CIMGUIComboBox* comboBox = AddWidget<CIMGUIComboBox>("ComboBox", 200.f, 30.f);
	comboBox->SetHideName(true);

	comboBox->AddItem("가");
	comboBox->AddItem("나");
	comboBox->AddItem("다");

	for (int i = 0; i < 100; ++i)
	{
		std::string name = "A" + std::to_string(i);
		comboBox->AddItem(name);
	}

	CIMGUIImage* image = AddWidget<CIMGUIImage>("Image", 300.f, 300.f);

	image->SetTexture("TeemoTest", TEXT("Teemo.jpg"));
	image->SetImageStart(0.f, 0.f);
	image->SetImageEnd(1215.f, 717.f);

	CIMGUICheckBox* checkBox = AddWidget<CIMGUICheckBox>("CheckBox", 300.f, 300.f);
	
	checkBox->AddCheckInfo("선");
	checkBox->AddCheckInfo("택");
	checkBox->AddCheckInfo("하");
	checkBox->AddCheckInfo("세");
	checkBox->AddCheckInfo("요");
	checkBox->AddCheckInfo("!!");
	
	text = AddWidget<CIMGUIText>("text2");
	text->SetText("========= 여러 줄 체크박스 ===========");

	checkBox = AddWidget<CIMGUICheckBox>("CheckBox2", 300.f, 300.f);
	checkBox->SetColNum(3);
	checkBox->AddCheckInfo("a");
	checkBox->AddCheckInfo("b");
	checkBox->AddCheckInfo("c");
	checkBox->AddCheckInfo("d");
	checkBox->AddCheckInfo("e");
	checkBox->AddCheckInfo("f");
	checkBox->AddCheckInfo("g");
	checkBox->AddCheckInfo("h");
	checkBox->AddCheckInfo("i");
	checkBox->AddCheckInfo("j");
	checkBox->AddCheckInfo("k");
	checkBox->AddCheckInfo("l");
	
	text = AddWidget<CIMGUIText>("text3");
	text->SetText("========= 다중 체크 X==========");

	CIMGUIRadioButton* radioButton = AddWidget<CIMGUIRadioButton>("RadioButton", 200.f, 200.f);
	radioButton->AddCheckInfo("초급");
	radioButton->AddCheckInfo("중급");
	radioButton->AddCheckInfo("고급");

	text = AddWidget<CIMGUIText>("text4");
	text->SetText("========= 다중 체크 O==========");

	radioButton = AddWidget<CIMGUIRadioButton>("RadioButton2", 200.f, 200.f);
	radioButton->SetMultiCheck(true);
	radioButton->AddCheckInfo("Red");
	radioButton->AddCheckInfo("Blue");
	radioButton->AddCheckInfo("Green");
	radioButton->AddCheckInfo("Alpha");
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
