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
			// 일을 가져온다 
			NavWorkData work = mWorkQueue.Front();
			mWorkQueue.Pop();

			// 처리
			NavResultData result;
			mNavigation->FindPath(work.Start, work.End, result.listPath);
			result.CallBack = work.CallBack;

			// 완료되면 매니저에 Result 넘긴다.
			mNavManager->AddNavResult(result);
		}
	}

	SetEvent(mExitEvent);
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* tileMap)
{
	mNavigation->CreateNavigationNodes(tileMap);
}
