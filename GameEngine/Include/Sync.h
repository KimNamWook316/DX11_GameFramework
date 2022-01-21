#pragma once

#include "GameInfo.h"

class CSync
{
public:
	// �Ӱ� ������ ���� ���� ������ �ش� �����尡 ��´�.
	// � �����忡 ���� �� �Լ��� �̹� ȣ��Ǿ� �ִٸ�, 
	// ȣ��� �Լ��� ���ŷ�ȴ�. ���� ������ ��ȯ�Ǹ�, ���ŷ ���¿� �ִ�
	// �Լ��� ���� ������ �ȴ�.
	CSync(CRITICAL_SECTION* crt)
	{
		mCrt = crt;
		EnterCriticalSection(mCrt);
	}
	
	// �� ������Ʈ�� �Ҹ�Ǹ鼭, �Ӱ� ������ ���� ���� ������ ��ȯ�ȴ�.
	~CSync()
	{
		LeaveCriticalSection(mCrt);
	}

private:
	CRITICAL_SECTION* mCrt;
};

