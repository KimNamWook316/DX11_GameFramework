#include "IMGUIColor4.h"

CIMGUIColor4::CIMGUIColor4()	:
	mValue{0.f, 0.f, 0.f, 0.f}
{
}

CIMGUIColor4::~CIMGUIColor4()
{
}

bool CIMGUIColor4::Init()
{
	return true;
}

void CIMGUIColor4::Render()
{
	if (ImGui::ColorEdit4(mName.c_str(), mValue))
	{
		if (mCallBack)
		{
			mCallBack(mValue);
		}
	}
}
