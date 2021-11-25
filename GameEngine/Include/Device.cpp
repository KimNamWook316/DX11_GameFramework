#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()
	: mDevice(nullptr)
	, mContext(nullptr)
	, mSwapChain(nullptr)
	, mTargetView(nullptr)
	, mDepthView(nullptr) 
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(mTargetView);
	SAFE_RELEASE(mDepthView);
	SAFE_RELEASE(mSwapChain);

	if (mContext) 
	{
		// Context�� Default Setting���� ������� ��.
		mContext->ClearState();
	}
	SAFE_RELEASE(mContext);
#ifdef _DEBUG
	mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&mDebug));
	//CheckMemoryLeak();
#endif // _DEBUG
	SAFE_RELEASE(mDebug);
	SAFE_RELEASE(mDevice);
}

bool CDevice::Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode) 
{
	mhWnd = hWnd; // � ������ �ڵ鿡 ����� ���ΰ�.
	mRS.Width = width;
	mRS.Height = height;

	// Text����� ���� BGRA Support
	unsigned int flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG

	flag |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	// ����� ���� ����, ����ü���� �����ϴ� ����ü
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������� ���� ����, 32��Ʈ
	swapDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapDesc.BufferDesc.RefreshRate.Denominator = 60; // 60 ������ ����
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // �������� �����Ӱ� ������ �� ����
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferCount = 1; // ����� �� ������ ��
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ���� OutPut�뵵�� ���
	swapDesc.OutputWindow = hWnd; // ��µ� ������
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = windowMode;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL fLevel1 = D3D_FEATURE_LEVEL_11_0;

	// 1. ����̽�, ����̽� ���ؽ�Ʈ, ����ü�� ����
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, flag, &fLevel, 1, D3D11_SDK_VERSION,
		&swapDesc, &mSwapChain, &mDevice, &fLevel1, &mContext))) 
	{
		assert(false);
		return false;
	}

	// SwapChain�� ������ �ִ� BackBuffer�� ���´�.
	ID3D11Texture2D* backBuffer = nullptr;

	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	// 2. ����Ÿ�ٺ� ����
	// BackBuffer�� ����� RenderTargetView�� �����Ѵ�. 
	mDevice->CreateRenderTargetView(backBuffer, nullptr, &mTargetView);

	// GetBuffer���� Ref Count �ϳ� ������Ű�Ƿ�, Release���ش�.
	SAFE_RELEASE(backBuffer);

	// 3. ���� ���� ����
	// ���̹��۸� �����.
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.ArraySize = 1;

	// 24Bit : Depth, 8bit : Stencil
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.MipLevels = 1;

	ID3D11Texture2D* depthBuffer = nullptr;

	// 4. Depth,Stencil �� ����
	mDevice->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
	mDevice->CreateDepthStencilView(depthBuffer, nullptr, &mDepthView);
	SAFE_RELEASE(depthBuffer);

	// 6. ����Ʈ ����
	D3D11_VIEWPORT vp = {};

	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MaxDepth = 1.f; // Depth���� 0~1���� ����

	// ����Ÿ���� ������ ���� ����, �����Ͷ����� ���������� �����̹Ƿ� RS�� ����
	// ����Ʈ�� �� ����Ÿ�ٺ��� �����Ѵ�.
	mContext->RSSetViewports(1, &vp);

	return true;
}

void CDevice::ClearRenderTarget(float clearColor[4])
{
	// ���� Ÿ�� �並 ���� ����۸� clearColor�� ä���.
	mContext->ClearRenderTargetView(mTargetView, clearColor);
}

void CDevice::ClearDepthStencil(float depth, unsigned char stencil)
{
	mContext->ClearDepthStencilView(mDepthView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		depth, stencil);
}

void CDevice::RenderStart()
{
	// ���� Ÿ�ٰ� ���� ���۸� Output Merger�ܰ迡 ���ε��Ѵ�.
	// �ټ��� ���� Ÿ���� �� �ܰ迡 ���ε� �� �� �ִ�.
	// ���� ���۴� �ϳ��� ����� �� ����
	mContext->OMSetRenderTargets(1, &mTargetView, mDepthView);
}

void CDevice::Flip()
{
	// ����۸� ����Ʈ���۷� �ٲٸ�, ������ ȭ�鿡 ǥ���Ѵ�.
	mSwapChain->Present(0, 0);
}
