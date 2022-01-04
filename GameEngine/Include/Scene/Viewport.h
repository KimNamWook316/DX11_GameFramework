#pragma once

#include "../GameInfo.h"
#include "../Widget/WidgetWindow.h"

class CViewport
{
	friend class CScene;

private:
	CViewport();
	~CViewport();

public:
	bool Init();
	void Start();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Render();

public:
	class CScene* GetScene() const
	{
		return mScene;
	}

public:
	template <typename T>
	T* FindWidgetWindow(const std::string& name)
	{
		auto iter = mWindowList.begin();
		auto iterEnd = mWindowList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (name == (*iter)->GetName())
			{
				return (T*)(*iter);
			}
		}
		return nullptr;
	}

	template <typename T>
	T* CreateWidgetWindow(const std::string& name)
	{
		T* window = new T;

		window->SetName(name);
		window->mViewport = this;

		if (!window->Init())
		{
			SAFE_RELEASE(window);
			return nullptr;
		}

		mWindowList.push_back((CWidgetWindow*)window);
		return window;
	}
	
private:
	class CScene* mScene;
	std::list<CSharedPtr<CWidgetWindow>> mWindowList;
};

