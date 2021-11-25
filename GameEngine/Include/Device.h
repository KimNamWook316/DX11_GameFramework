#pragma once

#include "GameInfo.h"

class CDevice 
{
public:
	bool Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);
	void ClearRenderTarget(float clearColor[4]);
	void ClearDepthStencil(float depth, unsigned char stencil);
	void RenderStart();
	void Flip();

	ID3D11Device* GetDevice() const
	{
		return mDevice;
	}

	ID3D11DeviceContext* GetContext() const
	{
		return mContext;
	}

	IDXGISwapChain* GetSwapChain() const
	{
		return mSwapChain;
	}

	Resolution GetResolution() const
	{
		return mRS;
	}

#ifdef _DEBUG
	// COM객체 메모리 누수 추적 함수
	void CheckMemoryLeak()
	{
		OutputDebugStringW(TEXT("############LEAK INFO############\n"));
		mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		OutputDebugStringW(TEXT("############LEAK INFO END############\n"));
	}
#endif // _DEBUG


	DECLARE_SINGLE(CDevice)

private:
	// 그래픽카드를 사용할 수 있게 만들어주는 Interface. Graphic Device와 응용 프로그램 간 동작
	// 컴퓨터마다 다른 그래픽 카드들을, 한 개의 그래픽 장치를 사용하듯 만들어준다.
	// ID3D11Device는 COM객체인 D3D11Device의 인터페이스 (객체가 가지고 있는 기능들을 가지고 있는 것)
	ID3D11Device* mDevice;

	// 프로그래밍에서 Device는 흔히 생각하는 그 장치가 아니라, 독립적인 오브젝트 (ex. 동영상 파일, 윈도우 창)
	// 과 같은 것들을 의미한다.
	// 그 Device에 대한 내용이 Device Context이다.
	// ID3D11DeviceContext란 즉, D3D11Device 내부 정보에 대한 인터페이스라고 이해하자.
	ID3D11DeviceContext* mContext;

	// DXGI : DirectX Graphic Infrastructure
	IDXGISwapChain* mSwapChain;

	// BackBuffer에 픽셀을 쓰기 위해 RenderTargetView를 만들고
	// BackBuffer를 이 TargetView에 연결해준다.
	ID3D11RenderTargetView* mTargetView;

	// depth, stencil값을 쓰기 위해 DepthStencilView를 만들고 연결해준다.
	ID3D11DepthStencilView* mDepthView;

	HWND mhWnd;
	Resolution mRS;

#ifdef _DEBUG
	ID3D11Debug* mDebug;
#endif // _DEBUG

};

