#pragma once

#include "GameInfo.h"

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

private:
	bool update(float deltaTime);
	bool postUpdate(float deltaTime);
	bool render(float deltaTime);

private:
	ATOM Register(const TCHAR* name, int iconID);
	BOOL Create(const TCHAR* name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine)

private:
	static bool mLoop;
	HINSTANCE	mhInst;
	HWND		mhWnd;
	Resolution	mRS;
	float		mClearColor[4];
};

