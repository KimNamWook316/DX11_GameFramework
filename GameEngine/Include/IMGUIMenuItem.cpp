#include "IMGUIMenuItem.h"

CIMGUIMenuItem::CIMGUIMenuItem()    :
    mbSelect(false),
    mbEnable(true)
{
}

CIMGUIMenuItem::~CIMGUIMenuItem()
{
}

bool CIMGUIMenuItem::Init()
{
    return true;
}

void CIMGUIMenuItem::Render()
{
    if (mShortCut.empty())
    {
        ImGui::MenuItem(mName.c_str(), NULL, &mbSelect, mbEnable);
    }
    else
    {
        ImGui::MenuItem(mName.c_str(), mShortCut.c_str(), &mbSelect, mbEnable);
    }

    if (mbSelect)
    {
        if (mCallBack)
        {
            mCallBack();
            mbSelect = false;
        }
    }
}
