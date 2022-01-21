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
	// �̺�Ʈ ����
	// p1 : ���� ���� p2 : ��� ���� ( Manual Reset, Auto Reset )
	// True�� ��� ����, False�� ��� �ڵ� ����
	// p3 : �̺�Ʈ ������Ʈ�� �ʱ� ���¸� ����, True�� ��� Signaled, False�� ��� non-signaled
	// p4 : �̺�Ʈ ������Ʈ�� �̸��� �� ��� ����ϴ� ��������
	mStartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// ������ ����
	// p1 : ���� ���� �� , p2 : �� �����忡 �Ҵ��ϴ� ���� ������ -> 0�̸� �ڵ� (default 1mb)
	// p3 : �������� ������ �Լ� p4 : ������� �Ѱ����� �Ķ���� ����
	// p5 : ������ ���� �÷��� ���� ( suspended ��� )
	// p6 : �������� ���� ��ȣ
	mThread = (HANDLE)_beginthreadex(nullptr, 0, CThread::threadFunction, this, 0, nullptr);
	return true;
}

void CThread::Start()
{
	// �� �����尡 Start�Ǹ�, �̺�Ʈ�� Signaled ���·� ��ȭ
	SetEvent(mStartEvent);
}

void CThread::Pause()
{
	DWORD count = 0;

	do
	{
		// ������� ī��Ʈ�� 0���� Ŭ ������ ������Ŵ
		count = SuspendThread(mThread);
	} while (count <= 0);
}

void CThread::Resume()
{
	DWORD count = 0;

	do
	{
		// ������� ī��Ʈ�� 0�� �ɋ����� ���ҽ�Ŵ
		count = ResumeThread(mThread);
	} while (count >= 0);
}

void CThread::waitForStartEvent()
{
	// Non-Signaled������ �̺�Ʈ ������Ʈ���, �� �Լ� ȣ���� ���ŷ�ȴ�.
	// mStartEvent�� Signaled���°� �� ������ ��ٸ���.
	WaitForSingleObject(mStartEvent, INFINITE);
}

// ������ ������ �Լ� ����
unsigned int __stdcall CThread::threadFunction(void* arg)
{
	CThread* thread = (CThread*)arg;
	
	thread->waitForStartEvent();
	thread->Run();
	return 0;
}
