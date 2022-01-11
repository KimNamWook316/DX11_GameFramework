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

public:
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

	ID2D1RenderTarget* Get2DRenderTarget() const
	{
		return m2DTarget;
	}

	ID2D1RenderTarget* Get2DWorldRenderTarget() const
	{
		return m2DTargetWorld;
	}

	ID2D1Factory* Get2DFactory() const
	{
		return m2DFactory;
	}

	Vector2 GetViewportAspectRatio();

#ifdef _DEBUG
	// COM��ü �޸� ���� ���� �Լ�
	void CheckMemoryLeak()
	{
		OutputDebugStringW(TEXT("############LEAK INFO############\n"));
		mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		OutputDebugStringW(TEXT("############LEAK INFO END############\n"));
	}
#endif // _DEBUG

	DECLARE_SINGLE(CDevice)

private:
	// �׷���ī�带 ����� �� �ְ� ������ִ� Interface. Graphic Device�� ���� ���α׷� �� ����
	// ��ǻ�͸��� �ٸ� �׷��� ī�����, �� ���� �׷��� ��ġ�� ����ϵ� ������ش�.
	// ID3D11Device�� COM��ü�� D3D11Device�� �������̽� (��ü�� ������ �ִ� ��ɵ��� ������ �ִ� ��)
	ID3D11Device* mDevice;

	// ���α׷��ֿ��� Device�� ���� �����ϴ� �� ��ġ�� �ƴ϶�, �������� ������Ʈ (ex. ������ ����, ������ â)
	// �� ���� �͵��� �ǹ��Ѵ�.
	// �� Device�� ���� ������ Device Context�̴�.
	// ID3D11DeviceContext�� ��, D3D11Device ���� ������ ���� �������̽���� ��������.
	ID3D11DeviceContext* mContext;

	// DXGI : DirectX Graphic Infrastructure
	IDXGISwapChain* mSwapChain;

	// BackBuffer�� �ȼ��� ���� ���� RenderTargetView�� �����
	// BackBuffer�� �� TargetView�� �������ش�.
	ID3D11RenderTargetView* mTargetView;

	// depth, stencil���� ���� ���� DepthStencilView�� ����� �������ش�.
	ID3D11DepthStencilView* mDepthView;

	HWND mhWnd;
	Resolution mRS;

	// Font ��� ���� Ÿ�� ( UI, ���� )
	ID2D1RenderTarget* m2DTarget;
	ID2D1RenderTarget* m2DTargetWorld;
	ID2D1Factory* m2DFactory;

#ifdef _DEBUG
	ID3D11Debug* mDebug;
#endif // _DEBUG
};

