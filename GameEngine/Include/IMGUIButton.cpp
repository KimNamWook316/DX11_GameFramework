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
