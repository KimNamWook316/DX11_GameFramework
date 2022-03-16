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

void CNavigationManager::ChangeTile(const int idx, eTileType eType)
{
	if (idx < 0 || idx > mNavData->GetPathFindTileCount() - 1)
	{
		return;
	}

	size_t size = mVecNavigationThread.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecNavigationThread[i]->AddChangeData(idx, eType);
	}

	// 디버깅, 피킹을 위해 타일맵 자체도 변경해준다.
	mNavData->SetPathFindTile(idx, eType);
}

int CNavigationManager::GetIndex(const Vector3& endPos)
{
	return mNavData->GetPathFindTileIndex(endPos);
}

bool CNavigationManager::IsReachableTile(const Vector3 pos)
{
	return mNavData->IsReachableTile(pos);
}

bool CNavigationManager::IsReachableTile(const int idx)
{
	return mNavData->IsReachableTile(idx);
}

PathFindTileInfo* CNavigationManager::GetPathFindTile(const int idx)
{
	return mNavData->GetPathFindTile(idx);
}

