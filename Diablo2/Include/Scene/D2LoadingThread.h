#pragma once

#include "Thread.h"
#include "ThreadQueue.h"
#include "../D2Info.h"

class CD2LoadingThread :
    public CThread
{
public:
    CD2LoadingThread();
    virtual ~CD2LoadingThread();

public:
    bool Init();
    virtual void Run() override;

public:
    CThreadQueue<D2LoadingMessage>* GetLoadingQueue()
    {
        return &mQueue;
    }

    void AddMessage(bool bComplete, const float percent)
    {
        D2LoadingMessage msg;
        msg.bComplete = bComplete;
        msg.Percent = percent;
        mQueue.Push(msg);
    }

public:
    CThreadQueue<D2LoadingMessage> mQueue;
};

