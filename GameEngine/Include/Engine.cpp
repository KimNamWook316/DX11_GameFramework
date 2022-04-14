#include "Engine.h"
#include "Device.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Collision/CollisionManager.h"
#include "PathManager.h"
#include "Input.h"
#include "Timer.h"
#include "IMGUIManager.h"
#include "Resource/Shader/GlobalConstantBuffer.h"
#include "Resource/Texture/Texture.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::mLoop = true;

CEngine::CEngine() 
	: mClearColor{0.0f, 0.5f, 0.0f, 0.5f}
	, mTimer(nullptr)
	, mbIsStart(false)
	, meSpace(eEngineSpace::Space2D)
	, mbPlay(true)
	, meMouseState(eMouseState::Normal)
	, mShowCursorCount(0)
	, mGlobalCBuffer(nullptr)
	, mGlobalAccTime(0.f),
	mbDebugMode(false)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(47300);
}

CEngine::~CEngine() 
{
	CSceneManager::DestroyInst();

	CInput::DestroyInst();
	CRenderManager::DestroyInst();
	CPathManager::DestroyInst();
	CCollisionManager::DestroyInst();
	CResourceManager::DestroyInst();
	CIMGUIManager::DestroyInst();
	SAFE_DELETE(mGlobalCBuffer);
	CDevice::DestroyInst();
	SAFE_DELETE(mTimer);
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* name, unsigned int width, 
	unsigned int height, int iconID, bool windowMode) 
{
	mhInst = hInst;

	mRS.Width = width;
	mRS.Height = height;

	Register(name, iconID);
	Create(name);

	return Init(hInst, mhWnd, width, height, windowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, 
	unsigned int width, unsigned int height, bool windowMode) 
{
	mhInst = hInst;

	mRS.Width = width;
	mRS.Height = height;

	mTimer = new CTimer;

	// Device �ʱ�ȭ
	if (!CDevice::GetInst()->Init(mhWnd, width, height, windowMode)) 
	{
		assert(false);
		return false;
	}

	// Path Manager
	if (!CPathManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// ���ҽ� ������ �ʱ�ȭ
	if (!CResourceManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// �浹 �Ŵ��� �ʱ�ȭ
	if (!CCollisionManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// ���� ������ �ʱ�ȭ
	if (!CRenderManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// �Է� �Ŵ���
	if (!CInput::GetInst()->Init(mhInst, mhWnd))
	{
		assert(false);
		return false;
	}

	// IMGUI �ʱ�ȭ
	if (!CIMGUIManager::GetInst()->Init(mhWnd))
	{
		assert(false);
		return false;
	}

	// Scene Manager
	if (!CSceneManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// Global Constant Buffer Init
	mGlobalCBuffer = new CGlobalConstantBuffer;
	if (!mGlobalCBuffer->Init())
	{
		assert(false);
		return false;
	}
	mGlobalCBuffer->SetResolution(mRS);

	// Noise Texture (for GPU Random) Init
	CResourceManager::GetInst()->LoadTexture("GlobalNoiseTexture", TEXT("noise_01.png"));
	mGlobalNoiseTexture = CResourceManager::GetInst()->FindTexture("GlobalNoiseTexture");
	mGlobalNoiseTexture->SetShader(100, (int)eBufferShaderTypeFlags::All, 0);
	mGlobalCBuffer->SetNoiseResolution((float)mGlobalNoiseTexture->GetWidth(), (float)mGlobalNoiseTexture->GetHeight());

	return true;
}

int CEngine::Run() 
{
	MSG msg;

	while (mLoop) 
	{
		// GetMessage�� �޼����� ���� ��� �ٸ����� �� �� ����.
		// �޼����� �ö����� ����ϰ� �ִ� �ð��� �������� ����Ÿ���̶�� �Ѵ�.
		// ������ �޼����� �ִ� �ð����� ���� �ð��� �ξ� ���.
		// �׷��� ������ �������� ����Ÿ�ӵ��� ������ ���۵� �� �ְ� �����Ѵ�.
		// PeekMessage�� �޼���ť���� �޼����� ���´�.
		// �׷��� ���� �޼����� ���ٸ� false�� �����ϸ鼭 �ٷ� ����������
		// �޼����� �ִٸ� true�� �����ϸ鼭 �ش� �޼����� �����´�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
		{
			// WM_KEYDOWN �̶�� �޼����� �ִ�. �̴� Ű���� Ű�� �������� �߻��Ǵ� �޼���
			// �̴�. TranslateMessage �Լ������� �޼����� �������ָ� WM_KEYDOWN ������
			// �Ǵ����ְ� ������ Ű�� ���� Ű������ �Ǵ��ؼ� �Ϲ� ���� Ű��� WM_CHAR���
			// �޽����� ���� �޼��� ť�� �߰����ְ� �ȴ�.
			TranslateMessage(&msg);

			// DispatchMessage �Լ��� �޼����� ������ ���ν����� �����ش�.
			DispatchMessage(&msg);
		}

		// �����찡 ����Ÿ���� ��� ����� ������ �ȴ�.
		else 
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	if (!mbIsStart)
	{
		mbIsStart = true;
		CSceneManager::GetInst()->Start();
	}

	mTimer->Update();

	float deltaTime = mTimer->GetDeltaTIme();

	// !play -> deltaTime == 0
	if (!mbPlay)
	{
		deltaTime = 0.f;
	}

	mGlobalAccTime += deltaTime;
	mGlobalCBuffer->SetDeltaTime(deltaTime);
	mGlobalCBuffer->SetAccTime(mGlobalAccTime);
	mGlobalCBuffer->UpdateCBuffer();

	CInput::GetInst()->Update(deltaTime);
	CResourceManager::GetInst()->Update();

	CIMGUIManager::GetInst()->Update(deltaTime);
	
	if (update(deltaTime))
	{
		return;
	}
	if (postUpdate(deltaTime))
	{
		return;
	}
	render(deltaTime);
}

void CEngine::SetMouseState(eMouseState state)
{
	if (mMouseWidget[(int)meMouseState])
	{
		// �ʱ�ȭ?
	}
	meMouseState = state;
}

bool CEngine::update(float deltaTime)
{
	if (CSceneManager::GetInst()->Update(deltaTime))
	{
		return true;
	}

	if (mMouseWidget[(int)meMouseState])
	{
		// ���콺 ��ġ�� ���´�.
		Vector2 mousePos = CInput::GetInst()->GetMousePos();

		// ���콺�� ������ â�� ��� ���, ������ �⺻ ���콺 Ŀ���� ���̰� �Ѵ�.
		if (mousePos.x < 0.f || mousePos.x > mRS.Width || mousePos.y < 0.f || mousePos.y > mRS.Height)
		{
			if (mShowCursorCount < 0)
			{
				// ������ ��
				// ShowCursor true �� ���� ����Ǹ�, ShowCursor false�� �� �� ����Ǿ�� ��������.
				// ���� �� ������ ó���� �� �ֵ��� ���� ��
				ShowCursor(true);
				mShowCursorCount = 0;
			}
		}
		else
		{
			if (mShowCursorCount == 0)
			{
				ShowCursor(false);
				--mShowCursorCount;
			}
		}

		// ���콺 ��ġ ����
		mousePos.y -= mMouseWidget[(int)meMouseState]->GetWindowSize().y;
		mMouseWidget[(int)meMouseState]->SetPos(mousePos);
		// ������Ʈ
		mMouseWidget[(int)meMouseState]->Update(deltaTime);
	}

	return false;
}

bool CEngine::postUpdate(float deltaTime)
{
	if (CSceneManager::GetInst()->PostUpdate(deltaTime))
	{
		return true;
	}
	
	if (mMouseWidget[(int)meMouseState])
	{
		mMouseWidget[(int)meMouseState]->PostUpdate(deltaTime);
	}
	return false;
}

bool CEngine::render(float deltaTime)
{
	CDevice::GetInst()->RenderStart();
	CDevice::GetInst()->ClearRenderTarget(mClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	CRenderManager::GetInst()->Render();
	
	CIMGUIManager::GetInst()->Render();

	CDevice::GetInst()->Flip();

	return false;
}

ATOM CEngine::Register(const TCHAR* name, int iconID) 
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	// �۾������� �ٲ� �ٽ� �׸��Բ� ���ش�. ���ο� ���� ��� �ٽ� �׸���.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// �޼����� ť�� �������� �ش� �޼����� �������� ������ �޼����� ���ڷ� �����ؼ�
	// ȣ������ �Լ��� �����Ѵ�.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance�� �����Ѵ�.
	wcex.hInstance = mhInst;

	// ���������� ��������� ����� ������ �̹����� �����Ѵ�.
	wcex.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(iconID));


	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	// �����ڵ� ���ڿ��� ����Ҷ� L"" �� �ٿ��� ����� �Ѵ�.
	// TEXT ��ũ�δ� "" �տ� L �� �ٿ��� L"" �� ������ش�.
	// �����ڵ��϶��� �̷��� ���ְ� multibyte  �϶��� �׳� "" �� ������ش�.
	wcex.lpszClassName = name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconID));

	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR* name) 
{
	mhWnd = CreateWindowW(name, name, WS_OVERLAPPEDWINDOW,
		0, 0, mRS.Width, mRS.Height, nullptr, nullptr, mhInst, nullptr);

	if (!mhWnd)
	{
		return FALSE;
	}

	// Ŭ���̾�Ʈ ������ 1280, 720���� ������ش�.
	// RECT : �簢���� ǥ���ϴ� ����ü�̴�.
	// left, top, right, bottom 4���� ������ �����Ǿ� �ִ�.
	RECT    rc = { 0, 0, (LONG)mRS.Width, (LONG)mRS.Height };

	// 1������ : Ŭ���̾�Ʈ ������ ũ�⸦ �����Ѵ�.
	// �� �Լ��� �Ϸ�Ǹ� rc���� ���� Ŭ���̾�Ʈ ������ ���ϴ� ũ�� �� �� �ִ�
	// ��ü �������� left, top, right, bottom ������ ������ �ȴ�.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ������ ���� Rect�� �̿��ؼ� ������ ũ�⸦ �����Ѵ�.
	SetWindowPos(mhWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOZORDER);

	// SW_SHOW : ������â�� ȭ�鿡 �����ش�.
	// SW_HIDE : â�� ����� ������ ȭ�鿡 �����ִ°� �ƴϴ�.
	ShowWindow(mhWnd, SW_SHOW);

	// UpdateWindow �Լ��� ������ �κ��� �ִٸ� �����϶�� ����� �����ִ� �Լ��̴�.
	UpdateWindow(mhWnd);

	return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return 1;
	}

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		// HDC : ȭ�鿡 ����ϱ� ���� �׸��� �����̴�.
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		mLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
