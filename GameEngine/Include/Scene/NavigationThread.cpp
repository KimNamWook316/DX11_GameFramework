#include "NavigationThread.h"
#include "NavigationManager.h"
#include "Navigation.h"

CNavigationThread::CNavigationThread()
{
	SetLoop(true);
	mNavigation = new CNavigation;
}

CNavigationThread::~CNavigationThread()
{
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
			mNavigation->FindPath(work.Start, work.End, result.vecPath);
			result.CallBack = work.CallBack;

			// 완료되면 매니저에 Result 넘긴다.
			mNavManager->AddNavResult(result);
		}
	}
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* tileMap)
{
	mNavigation->CreateNavigationNodes(tileMap);
}
