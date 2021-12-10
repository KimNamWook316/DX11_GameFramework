#include "IMGUICheckBox.h"

CIMGUICheckBox::CIMGUICheckBox()    :
    mbMultiColumn(false),
    mSpacingX(10.f)
{
}

CIMGUICheckBox::~CIMGUICheckBox()
{
    size_t size = mVecCheckInfo.size();
    
    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(mVecCheckInfo[i]);
    }

    mVecCheckInfo.clear();
}

bool CIMGUICheckBox::Init()
{
    return true;
}

void CIMGUICheckBox::Render()
{
    size_t size = mVecCheckInfo.size();

    int count = 0;

    for (size_t i = 0; i < size; ++i)
    {
        ImGui::Checkbox(mVecCheckInfo[i]->LabelUTF8, &mVecCheckInfo[i]->bCheck);

        if (mbMultiColumn)
        {
            if (count == mColNum - 1)
            {
                count -= mColNum - 1;
            }
            else
            {
                ImGui::SameLine(0.f, mSpacingX);
                ++count;
            }
        }
    }
}
