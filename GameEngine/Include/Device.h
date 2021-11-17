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
};

