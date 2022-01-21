#pragma once

#include "Thread.h"
#include "ThreadQueue.h"
#include "../Client.h"

class CLoadingThread :
    public CThread
{
public:
    CLoadingThread();
    virtual ~CLoadingThread();

public:
	bool Init();
	virtual void Run() override;

public:
    CThreadQueue<LoadingMessage>* GetLoadingQueue()
    {
        return &mQueue;
    }

    void AddMessage(bool bComplete, float percent)
    {
        LoadingMessage msg;
        msg.Complete = bComplete;
        msg.Percent = percent;
        mQueue.Push(msg);
    }

private:
    CThreadQueue<LoadingMessage> mQueue;
};

