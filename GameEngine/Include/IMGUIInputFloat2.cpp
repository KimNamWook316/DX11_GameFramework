#include "IMGUIInputFloat2.h"

CIMGUIInputFloat2::CIMGUIInputFloat2()	:
	mValue{0.f, 0.f},
	mbInputRange(false),
	mMin(0.f),
	mMax(100.f)
{
}

CIMGUIInputFloat2::~CIMGUIInputFloat2()
{
}

bool CIMGUIInputFloat2::Init()
{
	return true;
}

void CIMGUIInputFloat2::Render()
{
	if (ImGui::InputFloat2(mName.c_str(), mValue))
	{
		// 최대, 최소값이 있을 경우
		if (mbInputRange)
		{
			for (int i = 0; i < 2; ++i)
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
