#include "IMGUIWidgetList.h"

CIMGUIWidgetList::CIMGUIWidgetList()
{
}

CIMGUIWidgetList::~CIMGUIWidgetList()
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecChild[i]);
	}
}

bool CIMGUIWidgetList::Init()
{
    return false;
}

void CIMGUIWidgetList::Render()
{
	size_t size = mVecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->Render();
	}
}
