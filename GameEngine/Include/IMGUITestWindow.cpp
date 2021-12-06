#include "IMGUITestWindow.h"
#include "IMGUIButton.h"

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
	
	button = AddWidget<CIMGUIButton>("Button2");

	button->SetClickCallBack<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton2);
	
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
