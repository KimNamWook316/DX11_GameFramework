#include "Thread.h"

CThread::CThread()	:
	mThread(0),
	mStartEvent(0)
{
}

CThread::~CThread()
{
	if (mStartEvent)
	{
		CloseHandle(mStartEvent);
		mStartEvent = 0;
	}
}

bool CThread::Init()
{
	mStartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	mThread = (HANDLE)_beginthreadex(nullptr, 0, CThread::threadFunction, this, 0, nullptr);
	return true;
}

void CThread::Start()
{
	SetEvent(mStartEvent);
}

void CThread::Pause()
{
	DWORD count = 0;

	do
	{
		count = SuspendThread(mThread);
	} while (count <= 0);
}

void CThread::Resume()
{
	DWORD count = 0;

	do
	{
		count = ResumeThread(mThread);
	} while (count >= 0);
}

void CThread::waitForStartEvent()
{
	WaitForSingleObject(mStartEvent, INFINITE);
}

unsigned int __stdcall CThread::threadFunction(void* arg)
{
	CThread* thread = (CThread*)arg;
	
	thread->waitForStartEvent();
	thread->Run();
	return 0;
}
