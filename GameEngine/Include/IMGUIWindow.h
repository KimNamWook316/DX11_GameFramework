#pragma once

#include "GameInfo.h"

class CIMGUIWindow
{
public:
	CIMGUIWindow();
	virtual ~CIMGUIWindow();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

public:
	void Open();
	void Close();
	void ClosePopUp();
	void ClearWidget();
	class CIMGUIWidget* FindWidget(const std::string& name);

public:
	bool IsWindowFocused(ImGuiFocusedFlags_ eFlag = ImGuiFocusedFlags_None) const;

public:
	const unsigned long GetWindowID() const
	{
		return mWindowID;
	}

public:
	template <typename T>
	T* AddWidget(const std::string& name, const float width = 100.f, const float height = 100.f)
	{
		T* widget = new T;

		widget->SetName(name);
		widget->SetOwner(this);
		widget->SetSize(width, height);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			assert(false);
			return nullptr;
		}
		mVecWidget.push_back(widget);

		return widget;
	}

	template <typename T>
	T* AddPopUpWidget(const std::string& name, const float width = 100.f, const float height = 100.f)
	{
		T* widget = new T;
		
		widget->SetName(name);
		widget->SetOwner(this);
		widget->SetSize(width, height);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			assert(false);
			return nullptr;
		}
		mVecPopUpWidget.push_back(widget);
		return widget;
	}

public:
	void SetName(const std::string& name)
	{
		mName = name;
	}

	void AddWindowFlag(ImGuiWindowFlags_ eFlag)
	{
		mWindowFlag |= eFlag;
	}

	void EnableModalPopUp()
	{
		mbModalPopUp = true;
	}

	void SetPopUpWindowTitle(const std::string& name)
	{
		mPopUpTitle = name;
	}

protected:
	std::string mName;
	bool mbOpen;
	int mWindowFlag;
	std::vector<class CIMGUIWidget*> mVecWidget;

protected:
	std::string mPopUpTitle;
	bool mbModalPopUp;
	std::vector<class CIMGUIWidget*> mVecPopUpWidget;

protected:
	bool mbIsFocused;
	static unsigned long mWindowID;
};

