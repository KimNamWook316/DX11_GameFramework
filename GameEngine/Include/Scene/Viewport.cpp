#include "Viewport.h"
#include "../Input.h"

CViewport::CViewport()	:
	mScene(nullptr)
{
}

CViewport::~CViewport()
{
}

bool CViewport::Init()
{
	return true;
}

void CViewport::Start()
{
	auto iter = mWindowList.begin();
	auto iterEnd = mWindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CViewport::Update(float deltaTime)
{
	auto iter = mWindowList.begin();
	auto iterEnd = mWindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWindowList.erase(iter);
			iterEnd = mWindowList.end();
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

void CViewport::PostUpdate(float deltaTime)
{
	auto iter = mWindowList.begin();
	auto iterEnd = mWindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWindowList.erase(iter);
			iterEnd = mWindowList.end();
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

void CViewport::Render()
{
	// 출력 우선순위가 낮은 (뒤에 배치해야 하는) 위젯부터 출력하기 위해 역방향 순회
	auto iter = mWindowList.rbegin();
	auto iterEnd = mWindowList.rend();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}
		(*iter)->Render();
	}
}

bool CViewport::DoCollideMouse()
{
	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	// 출력 우선순위가 높은 위젯부터 충돌 수행하기 위해 Z오더로 내림차순 정렬
	if (mWindowList.size() >= 2)
	{
		mWindowList.sort(CViewport::sortWindow);
	}

	auto iter = mWindowList.begin();
	auto iterEnd = mWindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}

		if ((*iter)->DoCollideMouse(mousePos))
		{
			return true;
		}
	}

	// 충돌 이후 사라져야 할 UI가 있는 경우
	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mWindowList.erase(iter);
			iterEnd = mWindowList.end();
			continue;
		}
		++iter;
	}

	return false;
}

void CViewport::DoCollide()
{
}

bool CViewport::sortWindow(CSharedPtr<CWidgetWindow> src, CSharedPtr<CWidgetWindow> dest)
{
	return src->GetZOrder() > dest->GetZOrder();
}
