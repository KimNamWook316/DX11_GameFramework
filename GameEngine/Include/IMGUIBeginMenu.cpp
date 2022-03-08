#include "IMGUIBeginMenu.h"

CIMGUIBeginMenu::CIMGUIBeginMenu()
{
}

CIMGUIBeginMenu::~CIMGUIBeginMenu()
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecChild[i]);
	}
}

bool CIMGUIBeginMenu::Init()
{
	return true;
}

void CIMGUIBeginMenu::Render()
{
	if (ImGui::BeginMenu(mName.c_str()))
	{
		mSize = mVecChild.size();
		for (size_t i = 0; i < mSize; ++i)
		{
			mVecChild[i]->Render();
		}

		ImGui::EndMenu();
	}
}
