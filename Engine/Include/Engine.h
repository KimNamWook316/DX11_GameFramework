#pragma once

#include "GameInfo.h"
#include "Timer.h"
#include "Resource/Texture/Texture.h"

// 윈도우 생성 및 프로그램 로직, 디바이스 등 프로그램 전체를 관장하는 클래스
class CEngine
{
public:
	bool Init(HINSTANCE hInst, const TCHAR* name,
		unsigned int width, unsigned int height, int iconID,
		bool windowMode = true);

	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int Width,
		unsigned int height, bool windowMode = true);

	int Run();
	void Logic();

public:
	void Exit()
	{
		DestroyWindow(mhWnd);
	}

public:
	class CWidgetWindow* GetMouseWidget()
	{
		return mMouseWidget[(int)meMouseState];
	}

	HWND GetWindowHandle() const
	{
		return mhWnd;
	}

	HINSTANCE GetWindowInstance() const
	{
		return mhInst;
	}

	eEngineSpace GetEngineSpace() const
	{
		return meSpace;
	}

	bool IsPlay() const
	{
		return mbPlay;
	}

	float GetFPS() const
	{
		return mTimer->GetFPS();
	}

	float GetDeltaTime() const
	{
		return mTimer->GetDeltaTIme();
	}

	bool  IsDebugMode() const
	{
		return mbDebugMode;
	}

public:
	void SetMouseState(eMouseState state);

	void SetPlay(bool bPlay)
	{
		mbPlay = bPlay;
	}

	void SetEngineSpace(eEngineSpace space)
	{
		meSpace = space;
	}

	void SetDebugMode(bool bDebug)
	{
		mbDebugMode = bDebug;
	}

	void SetClearColor(const float r, const float g, const float b, const float a)
	{
		mClearColor[0] = r;
		mClearColor[1] = g;
		mClearColor[2] = b;
		mClearColor[3] = a;
	}

public:
	template <typename T>
	T* CreateMouse(eMouseState eState, const std::string& name)
	{
		T* window = new T;
		window->SetName(name);

		if (!window->Init())
		{
			SAFE_RELEASE(window);
			assert(false);
			return nullptr;
		}
		mMouseWidget[(int)eState] = window;
		return window;
	}

private:
	bool update(float deltaTime);
	bool postUpdate(float deltaTime);
	bool render(float deltaTime);

private:
	ATOM Register(const TCHAR* name, int iconID);
	BOOL Create(const TCHAR* name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// SingleTon 선언
	DECLARE_SINGLE(CEngine)

private:
	static bool mLoop;
	HINSTANCE	mhInst;
	HWND		mhWnd;
	Resolution	mRS;
	float		mClearColor[4]; // 화면 초기화 색상 변수
	class CTimer* mTimer;
	bool		mbIsStart;
	bool		mbPlay;
	eEngineSpace meSpace;
	eMouseState meMouseState;
	CSharedPtr<class CWidgetWindow> mMouseWidget[(int)eMouseState::Max];
	int mShowCursorCount;
	class CGlobalConstantBuffer* mGlobalCBuffer;
	float mGlobalAccTime; // 누적 시간
	CSharedPtr<CTexture> mGlobalNoiseTexture; // GPU 랜덤 함수를 위한 노이즈 텍스처
	bool mbDebugMode;
};

