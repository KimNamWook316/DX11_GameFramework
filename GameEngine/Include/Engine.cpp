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

	// Device 초기화
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

	// 리소스 관리자 초기화
	if (!CResourceManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// 충돌 매니저 초기화
	if (!CCollisionManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// 렌더 관리자 초기화
	if (!CRenderManager::GetInst()->Init())
	{
		assert(false);
		return false;
	}

	// 입력 매니저
	if (!CInput::GetInst()->Init(mhInst, mhWnd))
	{
		assert(false);
		return false;
	}

	// IMGUI 초기화
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
		// GetMessage는 메세지가 없을 경우 다른일을 할 수 없다.
		// 메세지가 올때까지 대기하고 있는 시간을 윈도우의 데드타임이라고 한다.
		// 실제로 메세지가 있는 시간보다 없는 시간이 훨씬 길다.
		// 그래서 게임은 윈도우의 데드타임동안 게임이 동작될 수 있게 제작한다.
		// PeekMessage는 메세지큐에서 메세지를 얻어온다.
		// 그런데 만약 메세지가 없다면 false를 리턴하면서 바로 빠져나오고
		// 메세지가 있다면 true를 리턴하면서 해당 메세지를 꺼내온다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
		{
			// WM_KEYDOWN 이라는 메세지가 있다. 이는 키보드 키를 눌렀을때 발생되는 메세지
			// 이다. TranslateMessage 함수에서는 메세지를 전달해주면 WM_KEYDOWN 인지를
			// 판단해주고 눌려진 키가 문자 키인지를 판단해서 일반 무자 키라면 WM_CHAR라는
			// 메시지를 만들어서 메세지 큐에 추가해주게 된다.
			TranslateMessage(&msg);

			// DispatchMessage 함수는 메세지를 윈도우 프로시저로 보내준다.
			DispatchMessage(&msg);
		}

		// 윈도우가 데드타임일 경우 여기로 들어오게 된다.
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
		// 초기화?
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
		// 마우스 위치를 얻어온다.
		Vector2 mousePos = CInput::GetInst()->GetMousePos();

		// 마우스가 윈도우 창을 벗어난 경우, 윈도우 기본 마우스 커서를 보이게 한다.
		if (mousePos.x < 0.f || mousePos.x > mRS.Width || mousePos.y < 0.f || mousePos.y > mRS.Height)
		{
			if (mShowCursorCount < 0)
			{
				// 주의할 점
				// ShowCursor true 가 세번 실행되면, ShowCursor false가 세 번 실행되어야 감춰진다.
				// 따라서 한 번씩만 처리할 수 있도록 해준 것
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

		// 마우스 위치 보정
		mousePos.y -= mMouseWidget[(int)meMouseState]->GetWindowSize().y;
		mMouseWidget[(int)meMouseState]->SetPos(mousePos);
		// 업데이트
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

	// 작업영역이 바뀔때 다시 그리게끔 해준다. 가로와 세로 모두 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지가 큐에 들어왔을때 해당 메세지를 꺼내오고 꺼내온 메세지를 인자로 전달해서
	// 호출해줄 함수를 지정한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance를 지정한다.
	wcex.hInstance = mhInst;

	// 실행파일이 만들어질때 사용할 아이콘 이미지를 지정한다.
	wcex.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(iconID));


	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	// 유니코드 문자열을 사용할때 L"" 을 붙여서 사용을 한다.
	// TEXT 매크로는 "" 앞에 L 을 붙여서 L"" 로 만들어준다.
	// 유니코드일때는 이렇게 해주고 multibyte  일때는 그냥 "" 로 만들어준다.
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

	// 클라이언트 영역을 1280, 720으로 만들어준다.
	// RECT : 사각형을 표현하는 구조체이다.
	// left, top, right, bottom 4개의 값으로 구성되어 있다.
	RECT    rc = { 0, 0, (LONG)mRS.Width, (LONG)mRS.Height };

	// 1번인자 : 클라이언트 영역의 크기를 전달한다.
	// 이 함수가 완료되면 rc에는 실제 클라이언트 영역이 원하는 크기 될 수 있는
	// 전체 윈도우의 left, top, right, bottom 정보가 들어오게 된다.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 위에서 얻어온 Rect를 이용해서 윈도우 크기를 지정한다.
	SetWindowPos(mhWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOZORDER);

	// SW_SHOW : 윈도우창을 화면에 보여준다.
	// SW_HIDE : 창을 만들긴 하지만 화면에 보여주는건 아니다.
	ShowWindow(mhWnd, SW_SHOW);

	// UpdateWindow 함수는 갱신할 부분이 있다면 갱신하라는 명령을 내려주는 함수이다.
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

		// HDC : 화면에 출력하기 위한 그리기 도구이다.
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
