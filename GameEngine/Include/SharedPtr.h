#pragma once

// 다양한 자료형을 담아야 하기 때문에 template로 선언
template <typename T>
class CSharedPtr
{
public:
	CSharedPtr()
		: mPtr(nullptr)
	{
	}

	CSharedPtr(T* ptr)
	{
		mPtr = ptr;

		if (mPtr)
		{
			mPtr->AddRef();
		}
	}

	CSharedPtr(const CSharedPtr<T>& ptr)
	{
		mPtr = ptr.mPtr;

		if (mPtr)
		{
			mPtr->AddRef();
		}
	}

	~CSharedPtr()
	{
		if (mPtr)
		{
			mPtr->Release();
		}
	}

public:
	void operator = (T* ptr)
	{
		if (mPtr)
		{
			mPtr->Release();
		}

		mPtr = ptr;

		if (mPtr)
		{
			mPtr->AddRef();
		}
	}

	void operator = (const CSharedPtr<T>& ptr)
	{
		if (mPtr)
		{
			mPtr->Release();
		}

		mPtr = ptr.mPtr;

		if (mPtr)
		{
			mPtr->AddRef();
		}
	}

	bool operator == (T* ptr) const
	{
		return mPtr == ptr;
	}

	bool operator == (const CSharedPtr<T>& ptr) const
	{
		return mPtr == ptr.mPtr;
	}

	bool operator != (T* ptr) const
	{
		return mPtr != ptr;
	}

	bool operator != (const CSharedPtr<T>& ptr) const
	{
		return mPtr != ptr.mPtr;
	}

	// type cast operator : T* 형으로 형변환 될 때
	operator T* () const
	{
		return mPtr;
	}

	// pointer operator
	T* operator -> () const
	{
		return mPtr;
	}

private:
	T* mPtr;
};