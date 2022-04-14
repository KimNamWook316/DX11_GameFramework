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

const Vector3& CNavigationManager::GetAroundEmptyTile(const Vector3& originPos)
{
	if (!mNavData)
	{
		return Vector3::Zero;
	}

	int originIdx = mNavData->GetTileIndex(originPos);
	int countX = mNavData->GetTileCountX();

	int originIdxX = originIdx % countX;
	int originIdxY = originIdx / countX;

	CTile* tile = nullptr;
	int emptyIdx = -1;
	int idx = -1;
	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}

			idx = (originIdxY + y) * countX + (originIdxX + x);

			tile = mNavData->GetTile(idx);

			if (tile && tile->GetTileType() == eTileType::Normal)
			{
				emptyIdx = idx;
				return tile->GetCenter();
			}
		}
	}
	return Vector3::Zero;
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

