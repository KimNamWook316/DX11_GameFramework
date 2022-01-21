#pragma once

#include "GameInfo.h"

class CThread
{
protected:
	CThread();
	virtual ~CThread() = 0;

public:
	bool Init();
	virtual void Run() = 0;

public:
	void Start();
	void Pause();
	void Resume();

private:
	void waitForStartEvent();

	// ��Ƽ������ �Լ� ����
	static unsigned int __stdcall threadFunction(void* arg);

public:
	template <typename T>
	static T* CreateThread(const std::string& name)
	{
		T* thread = new T;
		thread->mName = name;

		if (!thread->Init())
		{
			SAFE_DELETE(thread);
			return nullptr;
		}
		return thread;
	}

protected:
	std::string mName;
	HANDLE mThread;
	HANDLE mStartEvent;
};

