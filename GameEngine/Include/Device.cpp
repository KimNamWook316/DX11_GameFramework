#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()
	: mDevice(nullptr)
	, mContext(nullptr)
	, mSwapChain(nullptr)
	, mTargetView(nullptr)
	, mDepthView(nullptr) 
	, m2DTarget(nullptr)
	, m2DTargetWorld(nullptr)
	, m2DFactory(nullptr)
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m2DTarget);
	SAFE_RELEASE(m2DTargetWorld);
	SAFE_RELEASE(m2DFactory);

	SAFE_RELEASE(mTargetView);
	SAFE_RELEASE(mDepthView);
	SAFE_RELEASE(mSwapChain);

	if (mContext) 
	{
		// Context를 Default Setting으로 돌려줘야 함.
		mContext->ClearState();
	}
	SAFE_RELEASE(mContext);
#ifdef _DEBUG
	mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&mDebug));
	//CheckMemoryLeak();
	SAFE_RELEASE(mDebug);
#endif // _DEBUG
	SAFE_RELEASE(mDevice);
}

bool CDevice::Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode) 
{
	mhWnd = hWnd; // 어떤 윈도우 핸들에 출력할 것인가.
	mRS.Width = width;
	mRS.Height = height;

	// Text출력을 위한 BGRA Support
	unsigned int flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG

	flag |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG
	
	// 지원 레벨
	D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL fLevel1 = D3D_FEATURE_LEVEL_11_0;
	
	// 1. 디바이스, 디바이스 컨텍스트 생성
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0,
		flag, &fLevel, 1, D3D11_SDK_VERSION, &mDevice, &fLevel1, &mContext)))
	{
		assert(false);
		return false;
	}

	// 멀티샘플링 퀄리티 레벨 확인
	int availableSampleCount = 4;

	UINT checkerSampleCount = 0;
	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &checkerSampleCount);

	if (checkerSampleCount < 1)
	{
		availableSampleCount = 1;
	}

	// 백버퍼 세부 사항, 스왑체인을 정의하는 구조체
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 백버퍼의 포맷 설정, 32비트
	swapDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapDesc.BufferDesc.RefreshRate.Denominator = 60; // 60 프레임 설정
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 스케일이 자유롭게 조정될 수 있음
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferCount = 1; // 사용할 백 버퍼의 수
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟의 OutPut용도로 사용
	swapDesc.OutputWindow = hWnd; // 출력될 윈도우
	swapDesc.SampleDesc.Count = availableSampleCount;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = windowMode;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice* dxgiDevice = nullptr;
	mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* adapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter);

	IDXGIFactory* factory = nullptr;
	adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);

	// 스왑체인 생성
	if (FAILED(factory->CreateSwapChain(mDevice, &swapDesc, &mSwapChain)))
	{
		SAFE_RELEASE(dxgiDevice);
		SAFE_RELEASE(adapter);
		SAFE_RELEASE(factory);
		assert(false);
		return false;
	}

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	// SwapChain이 가지고 있는 BackBuffer를 얻어온다.
	ID3D11Texture2D* backBuffer = nullptr;

	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	// 2. 렌더타겟뷰 생성
	// BackBuffer와 연결된 RenderTargetView를 생성한다. 
	mDevice->CreateRenderTargetView(backBuffer, nullptr, &mTargetView);

	// GetBuffer에서 Ref Count 하나 증가시키므로, Release해준다.
	SAFE_RELEASE(backBuffer);

	// 3. 깊이 버퍼 생성
	// 깊이버퍼를 만든다.
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.ArraySize = 1;

	// 24Bit : Depth, 8bit : Stencil
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = availableSampleCount;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.MipLevels = 1;

	ID3D11Texture2D* depthBuffer = nullptr;

	// 4. Depth,Stencil 뷰 생성
	mDevice->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
	mDevice->CreateDepthStencilView(depthBuffer, nullptr, &mDepthView);
	SAFE_RELEASE(depthBuffer);

	// 6. 뷰포트 설정
	D3D11_VIEWPORT vp = {};

	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MaxDepth = 1.f; // Depth영역 0~1까지 설정

	// 렌더타겟의 렌더링 영역 설정, 래스터라이저 스테이지의 설정이므로 RS가 붙음
	// 뷰포트는 각 렌더타겟별로 설정한다.
	mContext->RSSetViewports(1, &vp);

	// 2D Factory 생성
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m2DFactory)))
	{
		assert(false);
		return false;
	}

	// 3D BackBuffer의 Surface를 얻어온다.
	IDXGISurface* backSurface = nullptr;
	mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backSurface));

	// 2D용 렌더타겟을 만들어준다.
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m2DFactory->CreateDxgiSurfaceRenderTarget(backSurface, props, &m2DTarget)))
	{
		assert(false);
		return false;
	}
	// Ref Count 증가한 surface release 처리
	SAFE_RELEASE(backSurface);

	return true;
}

void CDevice::ClearRenderTarget(float clearColor[4])
{
	// 렌더 타겟 뷰를 통해 백버퍼를 clearColor로 채운다.
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
	// 렌더 타겟과 깊이 버퍼를 Output Merger단계에 바인딩한다.
	// 다수의 렌더 타겟이 이 단계에 바인딩 될 수 있다.
	// 깊이 버퍼는 하나만 사용할 수 있음
	mContext->OMSetRenderTargets(1, &mTargetView, mDepthView);
}

void CDevice::Flip()
{
	// 백버퍼를 프론트버퍼로 바꾸며, 실제로 화면에 표시한다.
	mSwapChain->Present(0, 0);
}

Vector2 CDevice::GetViewportAspectRatio()
{
	RECT clientRC = {};
	GetClientRect(mhWnd, &clientRC);

	float width = (float)(clientRC.right - clientRC.left);
	float height = (float)(clientRC.bottom - clientRC.top);

	// 현재 크기에 대한 해상도 비율
	return Vector2(mRS.Width / width, mRS.Height / height);
}
