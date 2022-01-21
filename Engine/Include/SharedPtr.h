#pragma once

// �پ��� �ڷ����� ��ƾ� �ϱ� ������ template�� ����
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
		if (ptr)
		{
			ptr->AddRef();
		}

		if (mPtr)
		{
			mPtr->Release();
		}

		mPtr = ptr;
	}

	void operator = (const CSharedPtr<T>& ptr)
	{
		if (ptr.mPtr)
		{
			ptr.mPtr->AddRef();
		}
		
		if (mPtr)
		{
			mPtr->Release();
		}

		mPtr = ptr.mPtr;
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

	// type cast operator : T* ������ ����ȯ �� ��
	operator T* () const
	{
		return mPtr;
	}

	// pointer operator
	T* operator -> () const
	{
		return mPtr;
	}

	// SharedPtr�� ���� ������
	T* operator * () const
	{
		return mPtr;
	}

	T* Get()
	{
		return mPtr;
	}

private:
	T* mPtr;
};