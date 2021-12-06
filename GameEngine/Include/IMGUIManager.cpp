#include "IMGUIManager.h"
#include "IMGUIWindow.h"
#include "IMGUITestWindow.h"
#include "Device.h"

DEFINITION_SINGLE(CIMGUIManager)

CIMGUIManager::CIMGUIManager()	:
	mContext(nullptr)
{
}

CIMGUIManager::~CIMGUIManager()
{
	auto iter = mMapWindow.begin();
	auto iterEnd = mMapWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	auto iterFont = mMapFont.begin();
	auto iterEndFont = mMapFont.end();

	for (; iterFont != iterEndFont; ++iter)
	{
		SAFE_DELETE(iterFont->second);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool CIMGUIManager::Init(HWND hWnd)
{
	mContext = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	// BackEnd SetUp
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());
	
	CIMGUITestWindow* window = AddWindow<CIMGUITestWindow>("TestWindow");

	return true;
}

void CIMGUIManager::Update(float deltaTime)
{
	static bool bOpen = false;

//	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
//	{
//		bOpen = false;
//		CIMGUIWindow* window = FindIMGUIWindow("TestWindow");
//
//		if (window)
//		{
//			window->Open();
//		}
//	}
//
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	auto iter = mMapWindow.begin();
	auto iterEnd = mMapWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update(deltaTime);
	}

	// Window에서 그려낸 위젯들은 실제 화면에 그려지는 것이 아니라
	// IMGUI내부의 버퍼에 그려지게 된다.
	// 이 버퍼를 최종적으로 한 번 그려내는 방식으로 처리가 된다.
	ImGui::Render();
}

void CIMGUIManager::Render()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

CIMGUIWindow* CIMGUIManager::FindIMGUIWindow(const std::string& name)
{
	auto iter = mMapWindow.find(name);

	if (iter == mMapWindow.end())
	{
		return nullptr;
	}
	return iter->second;
}
