#pragma once

#include "GameInfo.h"

class CSync
{
public:
	// 임계 영역에 대한 접근 권한을 해당 스레드가 얻는다.
	// 어떤 스레드에 의해 이 함수가 이미 호출되어 있다면, 
	// 호출된 함수는 블로킹된다. 접근 권한이 반환되면, 블로킹 상태에 있던
	// 함수는 빠져 나오게 된다.
	CSync(CRITICAL_SECTION* crt)
	{
		mCrt = crt;
		EnterCriticalSection(mCrt);
	}
	
	// 이 오브젝트가 소멸되면서, 임계 영역에 대한 접근 권한이 반환된다.
	~CSync()
	{
		LeaveCriticalSection(mCrt);
	}

private:
	CRITICAL_SECTION* mCrt;
};

