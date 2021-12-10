#include "IMGUIComboBox.h"

CIMGUIComboBox::CIMGUIComboBox()    :
    mbSelect(false),
    mSelectIndex(-1),
    mPageItemCount(3),
    mFlag(0)
{
}

CIMGUIComboBox::~CIMGUIComboBox()
{
}

bool CIMGUIComboBox::Init()
{
	mPrevName = mName;
    return true;
}

void CIMGUIComboBox::Render()
{
    ImGui::PushItemWidth(mSize.x);

	// Begin ComboBox
	if (ImGui::BeginCombo(mName.c_str(), mPrevName.c_str(), mFlag))
	{
		size_t itemCount = mVecItemUTF8.size();

		for (size_t i = 0; i < itemCount; ++i)
		{
			mbSelect = (mSelectIndex == i);

			// Begin Selectable Items, Return when selected
			if (ImGui::Selectable(mVecItemUTF8[i].c_str(), mbSelect))
			{
				// Not Current Selected item, And CallBack is Enable
				if (mSelectIndex != i && mSelectCallBack)
				{
					// CallBack
					mSelectCallBack((int)i, mVecItem[i].c_str());
				}
				
				mPrevName = mVecItemUTF8[i];
				// Set Current Selected Index;
				mSelectIndex = (int)i;
			}

			if (mbSelect)
			{
				// Highlight SelectedItem
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}
