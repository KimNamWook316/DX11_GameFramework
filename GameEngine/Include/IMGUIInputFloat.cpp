#include "IMGUIInputFloat.h"

CIMGUIInputFloat::CIMGUIInputFloat()    :
    mVal(1.f),
    mStep(0.1f)
{
}

CIMGUIInputFloat::~CIMGUIInputFloat()
{
}

bool CIMGUIInputFloat::Init()
{
    return true;
}

void CIMGUIInputFloat::Render()
{
    ImGui::PushItemWidth(mSize.x);
    if (ImGui::InputFloat(mName.c_str(), &mVal, mStep))
    {
        if (mCallBack)
        {
            mCallBack(mVal);
        }
    }
    ImGui::PopItemWidth();
}
