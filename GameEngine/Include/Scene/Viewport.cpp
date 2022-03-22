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
	// ��� �켱������ ���� (�ڿ� ��ġ�ؾ� �ϴ�) �������� ����ϱ� ���� ������ ��ȸ
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

	// ��� �켱������ ���� �������� �浹 �����ϱ� ���� Z������ �������� ����
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

	// �浹 ���� ������� �� UI�� �ִ� ���
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
