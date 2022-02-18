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
    template <typename T>
    void AddWork(T* obj, void(T::* func)(const std::vector<Vector3>&),
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
    class CNavigation* mNavigation;
};

