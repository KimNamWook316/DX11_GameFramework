#include "IMGUIButton.h"

CIMGUIButton::CIMGUIButton() :
	mbClick(false)
{
}

CIMGUIButton::~CIMGUIButton()
{
}

bool CIMGUIButton::Init()
{
	SetSize(100.f, 50.f);
	return true;
}

void CIMGUIButton::Render()
{
	mbClick = ImGui::Button(mName.c_str(), mSize);

	if (mbClick && mClickCallBack)
	{
		mClickCallBack();
	}
}
