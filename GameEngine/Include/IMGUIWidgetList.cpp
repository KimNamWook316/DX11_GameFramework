#include "IMGUIWidgetList.h"

CIMGUIWidgetList::CIMGUIWidgetList()	:
	mSize(0)
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
	mSize = mVecChild.size();
	for (size_t i = 0; i < mSize; ++i)
	{
		mVecChild[i]->Render();
	}
}

void CIMGUIWidgetList::DeleteWidget(CIMGUIWidget* widget)
{
	auto iter = mVecChild.begin();
	auto iterEnd = mVecChild.end();

	for (; iter != iterEnd; ++iter)
	{
		if (widget == (*iter))
		{
			SAFE_DELETE(*iter);
			mVecChild.erase(iter);
			mSize = mVecChild.size();
			return;
		}
	}
}
