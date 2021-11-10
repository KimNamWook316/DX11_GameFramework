#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()
	: mDevice(nullptr)
	, mContext(nullptr)
	, mSwapChain(nullptr)
	, mTargetView(nullptr)
	, mDepthView(nullptr) {
}

CDevice::~CDevice(){
	SAFE_RELEASE(mTargetView);
	SAFE_RELEASE(mDepthView);
	SAFE_RELEASE(mSwapChain);

	if (mContext) {
		mContext->ClearState();
	}
	SAFE_RELEASE(mContext);
	SAFE_RELEASE(mDevice);
}

bool CDevice::Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode) {
	mhWnd = hWnd;
	mRS.Width = width;
	mRS.Height = height;

	unsigned int flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG

	flag |= D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = windowMode;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL fLevel1 = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, flag, &fLevel, 1, D3D11_SDK_VERSION,
		&swapDesc, &mSwapChain, &mDevice, &fLevel1, &mContext))) {
		return false;
	}

	return true;
}
