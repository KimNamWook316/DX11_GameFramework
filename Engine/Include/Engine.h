#pragma once

#include "GameInfo.h"

class CEngine {
public:
	bool Init(HINSTANCE hInst, const TCHAR* name, 
		unsigned int width, unsigned int height, int iconID, 
		bool windowMode = true);
	
	int Run();

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
};

