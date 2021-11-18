#include "Timer.h"

CTimer::CTimer()
	: mFPS(0)
	, mFPSTime(0)
	, mTick(0)
{
	// ���ػ� Ÿ�̸��� �ʴ� ƽ�� �����´�.
	QueryPerformanceFrequency(&mSecond);

	// ���ػ� Ÿ�̸��� ���� ƽ�� �����´�.
	QueryPerformanceCounter(&mTime);
}

CTimer::~CTimer()
{
}

float CTimer::Update()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	mDeltaTime = (curTime.QuadPart - mTime.QuadPart) / (float)mSecond.QuadPart;
	mTime = curTime;

	mFPSTime += mDeltaTime;
	++mTick;

	if (mTick == 60)
	{
		mFPS = 60 / mFPSTime;
		mFPSTime = 0.f;
		mTick = 0;
	}

	return mDeltaTime;
}