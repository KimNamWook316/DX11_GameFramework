#include "WidgetWindow.h"
#include "../Scene/Scene.h"
#include "../Component/WidgetComponent.h"

CWidgetWindow::CWidgetWindow() :
	mViewport(nullptr),
	mOwnerComponent(nullptr),
	mZOrder(0),
	mSize(100.f, 100.f),
	mbStart(false)
{
}

CWidgetWindow::CWidgetWindow(const CWidgetWindow& window)
{
	*this = window;
	mOwnerComponent = nullptr;

	auto iter = window.mWidgetList.begin();
	auto iterEnd = window.mWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		CWidget* widget = (*iter)->Clone();
		widget->mOwner = this;
		mWidgetList.push_back(widget);
	}
}

CWidgetWindow::~CWidgetWindow()
{
}

bool CWidgetWindow::Init()
{
	return true;
}

void CWidgetWindow::Start()
{
	mbStart = true;

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	if (mOwnerComponent)
	{
		mViewport = mOwnerComponent->GetScene()->GetViewport();
	}
}

void CWidgetWindow::Update(float deltaTime)
{
	if (!mbStart)
	{
		Start();
	}

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}
		
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		
		(*iter)->Update(deltaTime);
		++iter;
	}
}

void CWidgetWindow::PostUpdate(float deltaTime)
{
	if (!mbStart)
	{
		Start();
	}

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}
		
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		
		(*iter)->PostUpdate(deltaTime);
		++iter;
	}
}

void CWidgetWindow::Render()
{
	if (!mbStart)
	{
		Start();
	}

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWidgetList.erase(iter);
			iterEnd = mWidgetList.end();
			continue;
		}
		
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		
		(*iter)->Render();
		++iter;
	}
}

CWidgetWindow* CWidgetWindow::Clone()
{
	return new CWidgetWindow(*this);
}

bool CWidgetWindow::DoCollideMouse(const Vector2& mousePos)
{
	// Z order 내림차순 정렬
	mWidgetList.sort(&CWidgetWindow::sortWidget);

	// 윈도우 영역 안에 들어오는지 판단.
	if (mousePos.x < mPos.x)
	{
		return false;
	}
	else if (mousePos.x > mPos.x + mSize.x)
	{
		return false;
	}
	else if (mousePos.y < mPos.y)
	{
		return false;
	}
	else if (mousePos.y > mPos.y + mSize.y)
	{
		return false;
	}

	auto iter = mWidgetList.begin();
	auto iterEnd = mWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}

		// Hovered 처리
		if ((*iter)->DoCollideMouse(mousePos))
		{
			(*iter)->mbMouseHovered = true;
			return true;
		}
		else 
		{
			(*iter)->mbMouseHovered = false;
		}
	}

	return false;
}

bool CWidgetWindow::sortWidget(CSharedPtr<CWidget> src, CSharedPtr<CWidget> dest)
{
	return src->GetZOrder() > dest->GetZOrder();
}
