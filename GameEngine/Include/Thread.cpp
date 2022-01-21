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
	// 이벤트 생성
	// p1 : 보안 설정 p2 : 모드 설정 ( Manual Reset, Auto Reset )
	// True일 경우 수동, False일 경우 자동 리셋
	// p3 : 이벤트 오브젝트의 초기 상태를 결정, True일 경우 Signaled, False일 경우 non-signaled
	// p4 : 이벤트 오브젝트에 이름을 줄 경우 사용하는 전달인자
	mStartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// 스레드 생성
	// p1 : 보안 설정 값 , p2 : 이 스레드에 할당하는 스택 사이즈 -> 0이면 자동 (default 1mb)
	// p3 : 스레드의 진입점 함수 p4 : 스레드로 넘겨지는 파라미터 정보
	// p5 : 스레드 생성 플래그 정보 ( suspended 등등 )
	// p6 : 스레드의 고유 번호
	mThread = (HANDLE)_beginthreadex(nullptr, 0, CThread::threadFunction, this, 0, nullptr);
	return true;
}

void CThread::Start()
{
	// 이 스레드가 Start되면, 이벤트를 Signaled 상태로 변화
	SetEvent(mStartEvent);
}

void CThread::Pause()
{
	DWORD count = 0;

	do
	{
		// 서스펜드 카운트가 0보다 클 때까지 증가시킴
		count = SuspendThread(mThread);
	} while (count <= 0);
}

void CThread::Resume()
{
	DWORD count = 0;

	do
	{
		// 서스펜드 카운트가 0이 될떄까지 감소시킴
		count = ResumeThread(mThread);
	} while (count >= 0);
}

void CThread::waitForStartEvent()
{
	// Non-Signaled상태의 이벤트 오브젝트라면, 이 함수 호출이 블로킹된다.
	// mStartEvent가 Signaled상태가 될 떄까지 기다린다.
	WaitForSingleObject(mStartEvent, INFINITE);
}

// 스레드 진입점 함수 형식
unsigned int __stdcall CThread::threadFunction(void* arg)
{
	CThread* thread = (CThread*)arg;
	
	thread->waitForStartEvent();
	thread->Run();
	return 0;
}
