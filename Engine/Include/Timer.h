#pragma once

#include "GameInfo.h"

class CTimer
{
public:
	CTimer();
	~CTimer();

public:
	float GetDeltaTIme() const
	{
		return mDeltaTime;
	}

	float GetFPS() const
	{
		return mFPS;
	}

	float Update();
private:
	LARGE_INTEGER mTime;
	LARGE_INTEGER mSecond;

	float mDeltaTime;
	float mFPS;
	float mFPSTime;
	int mTick;
};

