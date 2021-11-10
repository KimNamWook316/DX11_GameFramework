#pragma once

#include "GameInfo.h"

class CDevice {
public:
	bool Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);
	DECLARE_SINGLE(CDevice)

private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	IDXGISwapChain* mSwapChain;

	// BackBuffer에 픽셀을 쓰기 위해 RenderTargetView를 만들고
	// BackBuffer를 이 TargetView에 연결해준다.
	ID3D11RenderTargetView* mTargetView;

	// depth, stencil값을 쓰기 위해 DepthStencilView를 만들고 연결해준다.
	ID3D11DepthStencilView* mDepthView;

	HWND mhWnd;
	Resolution mRS;
};

