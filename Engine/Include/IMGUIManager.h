#pragma once

#include "GameInfo.h"

class CIMGUIManager
{
public:
	bool Init(HWND hWnd);
	void Update(float deltaTime);
	void Render();
	class CIMGUIWindow* FindIMGUIWindow(const std::string& name);

public:
	template <typename T>
	T* AddWindow(const std::string& name)
	{
		T* window = (T*)FindIMGUIWindow(name);

		if (window)
		{
			return window;
		}

		window = new T;
		window->SetName(name);

		if (!window->Init())
		{
			SAFE_DELETE(window);
			assert(false);
			return nullptr;
		}
		mMapWindow.insert(std::make_pair(name, window));

		return window;
	}

	DECLARE_SINGLE(CIMGUIManager)

private:
	ImGuiContext* mContext;
	std::unordered_map<std::string, class CIMGUIWindow*> mMapWindow;
	std::unordered_map<std::string, ImFont*> mMapFont;
};

