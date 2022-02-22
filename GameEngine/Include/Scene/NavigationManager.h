#pragma once

#include "../Component/TileMapComponent.h"
#include "../ThreadQueue.h"
#include "NavigationThread.h"

class CNavigationManager
{
	friend class CScene;

private:
	CNavigationManager();
	~CNavigationManager();

public:
	bool Init();
	void Start();
	void Update(float deltaTime);

public:
	void SetNavData(CTileMapComponent* tileMap);
	void AddNavResult(const NavResultData& navData);

public:
	template <typename T>
	bool FindPath(T* obj, void(T::* func)(const std::list<Vector3>&),
		const Vector3& start, const Vector3& end)
	{
		if (mVecNavigationThread.empty())
		{
			return false;
		}

		// 일이 가장 적은 스레드에 넘긴다.
		int count = mVecNavigationThread[0]->GetWorkCount();
		int workIndex = 0;

		size_t size = mVecNavigationThread.size();

		for (size_t i = 1; i < size; ++i)
		{
			if (count > mVecNavigationThread[i]->GetWorkCount())
			{
				count = mVecNavigationThread[i]->GetWorkCount();
				workIndex = i;
			}
		}

		mVecNavigationThread[workIndex]->AddWork(obj, func, start, end);
		return true;
	}

private:
	class CScene* mScene;
	std::vector<CNavigationThread*> mVecNavigationThread;
	CSharedPtr<CTileMapComponent> mNavData;
	CThreadQueue<NavResultData> mResultQueue;
};

