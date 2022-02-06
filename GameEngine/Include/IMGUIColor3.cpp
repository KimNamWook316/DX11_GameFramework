#include "IMGUIColor3.h"

CIMGUIColor3::CIMGUIColor3()	:
	mValue{0.f, 0.f, 0.f}
{
}

CIMGUIColor3::~CIMGUIColor3()
{
}

bool CIMGUIColor3::Init()
{
	return true;
}

void CIMGUIColor3::Render()
{
	if (ImGui::ColorEdit3(mName.c_str(), mValue))
	{
		if (mCallBack)
		{
			mCallBack(mValue);
		}
	}
}
