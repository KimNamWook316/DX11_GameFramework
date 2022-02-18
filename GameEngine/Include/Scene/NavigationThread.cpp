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
			// ���� �����´� 
			NavWorkData work = mWorkQueue.Front();
			mWorkQueue.Pop();

			// ó��
			NavResultData result;
			mNavigation->FindPath(work.Start, work.End, result.vecPath);
			result.CallBack = work.CallBack;

			// �Ϸ�Ǹ� �Ŵ����� Result �ѱ��.
			mNavManager->AddNavResult(result);
		}
	}
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* tileMap)
{
	mNavigation->CreateNavigationNodes(tileMap);
}
