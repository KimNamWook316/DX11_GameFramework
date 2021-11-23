#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	virtual ~CRef();

public:
	void Enable(bool bEnable)
	{
		mEnable = bEnable;
	}

	void Destroy()
	{
		mEnable = false;
	}

	bool IsActive() const
	{
		return mActive;
	}

	bool IsEnable() const
	{
		return mEnable;
	}

	void AddRef()
	{
		++mRefCount;
	}

	int GetRefCount() const
	{
		return mRefCount;
	}

	int Release()
	{
		--mRefCount;
		
		if (mRefCount <= 0)
		{
			delete this;
			return 0;
		}
		
		return mRefCount;
	}

	void SetName(const std::string& name)
	{
		mName = name;
	}

	const std::string& GetName() const
	{
		return mName;
	}

	template <typename T>
	void SetTypeID()
	{
		mTypeID = typeid(T).hash_code();
	}

	template <typename T>
	bool CheckType()
	{
		return mTypeID == typeid(T).hash_code();
	}

protected:
	std::string mName;
	int			mRefCount;
	bool		mEnable;	// �� ��ü�� Destroy �Ǿ�� �ϴ���
	bool		mActive;	// �� ��ü�� Ȱ��ȭ ��������
	size_t		mTypeID;	// Ŭ������ TypeID
};

