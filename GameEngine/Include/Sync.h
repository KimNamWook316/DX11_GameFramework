#pragma once

#include "GameInfo.h"

class CSync
{
public:
	CSync(CRITICAL_SECTION* crt)
	{
		mCrt = crt;
		EnterCriticalSection(mCrt);
	}
	
	~CSync()
	{
		LeaveCriticalSection(mCrt);
	}

private:
	CRITICAL_SECTION* mCrt;
};

