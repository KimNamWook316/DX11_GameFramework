#pragma once

#include "IMGUIWidget.h"

class CIMGUIInputFloat3 :
    public CIMGUIWidget
{
public:
	CIMGUIInputFloat3();
	virtual ~CIMGUIInputFloat3();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetVal(const float x, const float y, const float z)
	{
		mValue[0] = x;
		mValue[1] = y;
		mValue[2] = z;
	}

	void SetX(const float x)
	{
		mValue[0] = x;
	}

	void SetY(const float y)
	{
		mValue[1] = y;
	}

	void SetZ(const float z)
	{
		mValue[2] = z;
	}

	void EnableInputRange(bool bEnable)
	{
		mbInputRange = bEnable;
	}

	void SetMin(const float val)
	{
		mMin = val;
	}
	
	void SetMax(const float val)
	{
		mMax = val;
	}

public:
	float GetX() const
	{
		return mValue[0];
	}

	float GetY() const
	{
		return mValue[1];
	}

	float GetZ() const
	{
		return mValue[2];
	}

public:
	template <typename T>
	void SetCallBack(T* obj, void(T::* func)(float[3]))
	{
		mCallBack = std::bind(func, obj, std::placeholders::_1);
	}

protected:
	float mValue[3];
	std::function<void(float[3])> mCallBack;
	bool mbInputRange;
	float mMin;
	float mMax;
};

