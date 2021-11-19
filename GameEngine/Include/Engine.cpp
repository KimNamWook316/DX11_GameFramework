#include "Engine.h"
#include "Device.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "PathManager.h"
#include "Timer.h"
//#include "Resource/Mesh/Mesh.h";
//#include "Resource/Shader/Shader.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::mLoop = true;

CEngine::CEngine() 
	: mClearColor{}
	, mTimer(nullptr)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(243);
}

CEngine::~CEngine() 
{
	CSceneManager::DestroyInst();
	CPathManager::DestroyInst();
	CResourceManager::DestroyInst();
	CDevice::DestroyInst();
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

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, unsigned int width, unsigned int height, bool windowMode) 
{
	mhInst = hInst;

	mRS.Width = width;
	mRS.Height = height;

	mTimer = new CTimer;

	// Device �ʱ�ȭ
	if (!CDevice::GetInst()->Init(mhWnd, width, height, windowMode)) 
	{
		return false;
	}

	// Path Manager
	if (!CPathManager::GetInst()->Init())
	{
		return false;
	}

	// ���ҽ� ������ �ʱ�ȭ
	if (!CResourceManager::GetInst()->Init())
	{
		return false;
	}

	// Scene Manager
	if (!CSceneManager::GetInst()->Init())
	{
		return false;
	}

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
	mTimer->Update();

	float deltaTime = mTimer->GetDeltaTIme();

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

bool CEngine::update(float deltaTime)
{
	return false;
}

bool CEngine::postUpdate(float deltaTime)
{
	return false;
}

bool CEngine::render(float deltaTime)
{
	CDevice::GetInst()->RenderStart();
	CDevice::GetInst()->ClearRenderTarget(mClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	//CMesh* mesh = CResourceManager::GetInst()->FindMesh("SpriteMesh");
	//CShader* shader = CResourceManager::GetInst()->FindShader("ColorMeshShader");

	//shader->SetShader();
	//mesh->Render();
	
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

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
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
