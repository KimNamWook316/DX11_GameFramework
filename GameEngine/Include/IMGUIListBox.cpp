#include "IMGUIListBox.h"

CIMGUIListBox::CIMGUIListBox() : 
	mbSelect(false),
	mSelectIndex(-1),
	mPageItemCount(3)
{
}

CIMGUIListBox::~CIMGUIListBox()
{
}

bool CIMGUIListBox::Init()
{
	return true;
}

void CIMGUIListBox::Render()
{
	ImVec2 size = ImVec2(mSize.x, mPageItemCount * ImGui::GetTextLineHeightWithSpacing());

	// Begin ListBox
	if (ImGui::BeginListBox(mName.c_str(), size))
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

				// Set Current Selected Index;
				mSelectIndex = (int)i;
			}

			if (mbSelect)
			{
				// Highlight SelectedItem
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}
