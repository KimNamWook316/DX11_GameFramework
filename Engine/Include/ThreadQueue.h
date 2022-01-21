#pragma once

#include "GameInfo.h"
#include "Sync.h"

template <typename T, int SIZE = 200>
class CThreadQueue
{
public:
	CThreadQueue()
	{
		mSize = 0;
		mHead = 0;
		mTail = 0;
		mCapacity = SIZE + 1;

		InitializeCriticalSection(&mCrt);
	}

	~CThreadQueue()
	{
		DeleteCriticalSection(&mCrt);
	}

public:
	void Push(const T& data)
	{
		CSync sync(&mCrt);

		int tail = (mTail + 1) % mCapacity;

		if (tail == mHead)
		{
			return;
		}

		mQueue[tail] = data;
		mTail = tail;
		++mSize;
	}

	T& Front()
	{
		if (Empty())
		{
			assert(false);
		}

		CSync sync(&mCrt);

		int head = (mHead + 1) % mCapacity;

		return mQueue[head];
	}

	void Pop()
	{
		if (Empty())
		{
			assert(false);
		}
		
		CSync sync(&mCrt);

		mHead = (mHead + 1) % mCapacity;
		--mSize;
	}

	int Size()
	{
		CSync sync(&mCrt);
		
		return mSize;
	}

	bool Empty()
	{
		CSync sync(&mCrt);

		return mSize == 0;
	}

	void Clear()
	{
		CSync sync(&mCrt);

		mHead = 0;
		mTail = 0;
		mSize = 0;
	}

private:
	T mQueue[SIZE + 1];
	int mCapacity;
	int mSize;
	int mHead;
	int mTail;
	CRITICAL_SECTION mCrt;
};

