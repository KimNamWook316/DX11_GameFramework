#include "IMGUIText.h"

CIMGUIText::CIMGUIText()	:
	mText{},
	mwText{},
	mTextUTF8{}
{
}

CIMGUIText::~CIMGUIText()
{
}

bool CIMGUIText::Init()
{
	return true;
}

void CIMGUIText::Render()
{
	ImGui::TextColored(mColor.Value, mTextUTF8);
}
