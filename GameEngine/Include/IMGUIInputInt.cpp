#include "IMGUIInputInt.h"

CIMGUIInputInt::CIMGUIInputInt()    :
    mVal(0),
    mStep(1)
{
}

CIMGUIInputInt::~CIMGUIInputInt()
{
}

bool CIMGUIInputInt::Init()
{
    return true;
}

void CIMGUIInputInt::Render()
{
    ImGui::PushItemWidth(mSize.x);
    ImGui::InputInt(mName.c_str(), &mVal, mStep);
    ImGui::PopItemWidth();
}
