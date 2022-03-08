#include "NavigationManager.h"

CNavigationManager::CNavigationManager()
{
}

CNavigationManager::~CNavigationManager()
{
	size_t size = mVecNavigationThread.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecNavigationThread[i]);
	}

	mVecNavigationThread.clear();
}

bool CNavigationManager::Init()
{
	return true;
}

void CNavigationManager::Start()
{
}

void CNavigationManager::Update(float deltaTime)
{
	if (!mResultQueue.Empty())
	{
		NavResultData result = mResultQueue.Front();
		mResultQueue.Pop();

		result.CallBack(result.listPath);
	}
}

void CNavigationManager::SetNavData(CTileMapComponent* tileMap)
{
	mNavData = tileMap;

	if (!mVecNavigationThread.empty())
	{
		size_t size = mVecNavigationThread.size();

		for (size_t i = 0; i < size; ++i)
		{
			SAFE_DELETE(mVecNavigationThread[i]);
		}

		mVecNavigationThread.clear();
	}

	if (mNavData)
	{
		// 4개 스레드 생성 후 바로 실행
		for (int i = 0; i < 4; ++i)
		{
			char name[256] = {};
			sprintf_s(name, "NavThread%d", i);
			CNavigationThread* thread = CThread::CreateThread<CNavigationThread>(name);

			thread->mNavManager = this;
			thread->CreateNavigationNode(mNavData);
			thread->Start();

			mVecNavigationThread.push_back(thread);
		}
	}
}

void CNavigationManager::AddNavResult(const NavResultData& navData)
{
	mResultQueue.Push(navData);
}

int CNavigationManager::GetIndex(const Vector3& endPos)
{
	return mNavData->GetPathFindTileIndex(endPos);
}
