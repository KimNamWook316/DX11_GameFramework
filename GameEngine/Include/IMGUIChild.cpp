#include "IMGUIChild.h"

CIMGUIChild::CIMGUIChild() :
	mbBorder(false)
{
}

CIMGUIChild::~CIMGUIChild()
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecChild[i]);
	}
}

bool CIMGUIChild::Init()
{
	return true;
}

void CIMGUIChild::Render()
{
	ImGui::BeginChild(mName.c_str(), mSize, mbBorder);

	size_t size = mVecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->Render();
	}

	ImGui::EndChild();
}
