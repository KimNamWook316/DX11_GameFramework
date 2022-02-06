#pragma once

#include "IMGUIWidget.h"

class CIMGUIColor4 :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUITree;
	friend class CIMGUIChild;
    friend class CIMGUIWidgetList;
    
protected:
	CIMGUIColor4();
	virtual ~CIMGUIColor4();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetRGBA(const float r, const float g, const float b, const float a)
	{
		mValue[0] = r;
		mValue[1] = g;
		mValue[2] = b;
		mValue[3] = a;
	}

	void SetR(const float r)
	{
		mValue[0] = r;
	}

	void SetG(const float g)
	{
		mValue[1] = g;
	}

	void SetB(const float b)
	{
		mValue[2] = b;
	}

	void SetA(const float a)
	{
		mValue[3] = a;
	}

public:
	float GetR() const
	{
		return mValue[0];
	}

	float GetG() const
	{
		return mValue[1];
	}

	float GetB() const
	{
		return mValue[2];
	}

	float GetA() const
	{
		return mValue[3];
	}

public:
	template <typename T>
	void SetCallBack(T* obj, void(T::* func)(float[4]))
	{
		mCallBack = std::bind(func, obj, std::placeholders::_1);
	}

protected:
	float mValue[4];
	std::function<void(float[4])> mCallBack;
};

