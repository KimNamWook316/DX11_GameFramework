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

	// BackBuffer�� �ȼ��� ���� ���� RenderTargetView�� �����
	// BackBuffer�� �� TargetView�� �������ش�.
	ID3D11RenderTargetView* mTargetView;

	// depth, stencil���� ���� ���� DepthStencilView�� ����� �������ش�.
	ID3D11DepthStencilView* mDepthView;

	HWND mhWnd;
	Resolution mRS;
};

