#pragma once
#include "IMGUIWidget.h"
class CIMGUIColor3 :
    public CIMGUIWidget
{
public:
	CIMGUIColor3();
	virtual ~CIMGUIColor3();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetRGB(const float r, const float g, const float b)
	{
		mValue[0] = r;
		mValue[1] = g;
		mValue[2] = b;
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

public:
	template <typename T>
	void SetCallBack(T* obj, void(T::* func)(float[3]))
	{
		mCallBack = std::bind(func, obj, std::placeholders::_1);
	}

protected:
	float mValue[3];
	std::function<void(float[3])> mCallBack;
};

