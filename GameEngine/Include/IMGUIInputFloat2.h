#pragma once

#include "IMGUIWidget.h"

class CIMGUIInputFloat2 :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUITree;
	friend class CIMGUIChild;
    friend class CIMGUIWidgetList;

protected:
	CIMGUIInputFloat2();
	virtual ~CIMGUIInputFloat2();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetVal(const float x, const float y)
	{
		mValue[0] = x;
		mValue[1] = y;
	}

	void SetX(const float x)
	{
		mValue[0] = x;
	}

	void SetY(const float y)
	{
		mValue[1] = y;
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

public:
	template <typename T>
	void SetCallBack(T* obj, void(T::* func)(float[2]))
	{
		mCallBack = std::bind(func, obj, std::placeholders::_1);
	}

protected:
	float mValue[2];
	std::function<void(float[2])> mCallBack;
	bool mbInputRange;
	float mMin;
	float mMax;
};

