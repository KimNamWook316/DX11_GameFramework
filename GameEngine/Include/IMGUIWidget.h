#pragma once

#include "GameInfo.h"

class CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIWidget();
	virtual ~CIMGUIWidget();

public:
	virtual bool Init();
	virtual void Render();

public:
	void SetOwner(class CIMGUIWindow* owner)
	{
		mOwner = owner;
	}
	
	void SetName(const std::string& name)
	{
		mName = name;
	}
	
	void SetHideName(const bool bHide)
	{
		if (bHide)
		{
			mName = "##" + mName;
		}
	}

	void SetSize(const ImVec2& size)
	{
		mSize = size;
	}

	void SetSize(const Vector2& size)
	{
		mSize.x = size.x;
		mSize.y = size.y;
	}

	void SetSize(const float x, const float y)
	{
		mSize.x = x;
		mSize.y = y;
	}

	void SetColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255)
	{
		mColor.Value.x = r / 255.f;
		mColor.Value.y = g / 255.f;
		mColor.Value.z = b / 255.f;
		mColor.Value.w = a / 255.f;
	}

protected:
	std::string mName;
	class CIMGUIWindow* mOwner;
	ImVec2 mSize;
	ImColor mColor;
	ImFont* mFont;
}; 