#include "IMGUISameLine.h"

CIMGUISameLine::CIMGUISameLine()	:
	mOffsetX(0.f),
	mSpacing(-1.f)
{
}

CIMGUISameLine::~CIMGUISameLine()
{
}

bool CIMGUISameLine::Init()
{
	return true;
}

void CIMGUISameLine::Render()
{
	ImGui::SameLine(mOffsetX, mSpacing);
}
