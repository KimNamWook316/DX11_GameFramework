#include "NavigationThread.h"
#include "NavigationManager.h"
#include "Navigation.h"

CNavigationThread::CNavigationThread()
{
	SetLoop(true);
	mNavigation = new CNavigation;
	mExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CNavigationThread::~CNavigationThread()
{
	mbLoop = false;
	WaitForSingleObject(mExitEvent, INFINITE);
	CloseHandle(mExitEvent);
	SAFE_DELETE(mNavigation);
}

void CNavigationThread::Run()
{
	while(true)
	{
		if (!mWorkQueue.Empty())
		{
			// ���� �����´� 
			NavWorkData work = mWorkQueue.Front();
			mWorkQueue.Pop();

			// ó��
			NavResultData result;
			mNavigation->FindPath(work.Start, work.End, result.listPath);
			result.CallBack = work.CallBack;

			// �Ϸ�Ǹ� �Ŵ����� Result �ѱ��.
			mNavManager->AddNavResult(result);
		}
	}

	SetEvent(mExitEvent);
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* tileMap)
{
	mNavigation->CreateNavigationNodes(tileMap);
}
