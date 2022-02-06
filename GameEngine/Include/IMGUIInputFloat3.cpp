#include "IMGUIInputFloat3.h"

CIMGUIInputFloat3::CIMGUIInputFloat3()	:
	mValue{0.f, 0.f, 0.f},
	mbInputRange(false),
	mMin(0.f),
	mMax(100.f)
{
}

CIMGUIInputFloat3::~CIMGUIInputFloat3()
{
}

bool CIMGUIInputFloat3::Init()
{
	return true;
}

void CIMGUIInputFloat3::Render()
{
	if (ImGui::InputFloat3(mName.c_str(), mValue))
	{
		// 최대, 최소값이 있을 경우
		if (mbInputRange)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (mValue[i] < mMin)
				{
					mValue[i] = mMin;
				}
				else if (mValue[i] > mMax)
				{
					mValue[i] = mMax;
				}
			}
		}

		if (mCallBack)
		{
			mCallBack(mValue);
		}
	}
}
