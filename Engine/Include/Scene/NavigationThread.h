#pragma once

#include "../Thread.h"
#include "../ThreadQueue.h"

class CNavigationThread :
    public CThread
{
    friend class CNavigationManager;

public:
    CNavigationThread();
    virtual ~CNavigationThread();

public:
    virtual void Run() override;

public:
    void CreateNavigationNode(class CTileMapComponent* tileMap);

public:
    int GetWorkCount()
    {
        return mWorkQueue.Size();
    }

public:
    void AddChangeData(const int idx, eTileType type)
    {
        NavChangeData data;
        data.Index = idx;
        data.Type = type;
        mChangeQueue.Push(data);
    }

public:
    template <typename T>
    void AddWork(T* obj, void(T::* func)(const std::list<Vector3>&),
        const Vector3& start, const Vector3& end)
    {
        NavWorkData data;
        data.CallBack = std::bind(func, obj, std::placeholders::_1);
        data.Start = start;
        data.End = end;

        mWorkQueue.Push(data);
    }

private:
    class CNavigationManager* mNavManager;
    CThreadQueue<NavWorkData> mWorkQueue;
    CThreadQueue<NavChangeData> mChangeQueue;
    class CNavigation* mNavigation;
    HANDLE mExitEvent;
};

