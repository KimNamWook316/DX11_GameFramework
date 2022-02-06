#include "IMGUISliderFloat.h"

CIMGUISliderFloat::CIMGUISliderFloat()  :
    mValue(0.f),
    mMin(0.f),
    mMax(1.f)
{
}

CIMGUISliderFloat::~CIMGUISliderFloat()
{
}

bool CIMGUISliderFloat::Init()
{
    return true;
}

void CIMGUISliderFloat::Render()
{
    if (ImGui::SliderFloat(mName.c_str(), &mValue, mMin, mMax, "%.1f", 0))
    {
        if (mCallBack)
        {
            mCallBack(mValue);
        }
    }
}
