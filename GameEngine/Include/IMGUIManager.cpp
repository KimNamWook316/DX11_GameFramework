#include "IMGUIManager.h"
#include "IMGUIWindow.h"
#include "IMGUITestWindow.h"
#include "Device.h"
#include "PathManager.h"

DEFINITION_SINGLE(CIMGUIManager)

CIMGUIManager::CIMGUIManager()	:
	mContext(nullptr),
	mCurrentFont(nullptr)
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

	// Multi-Vieports
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// TODO : 임시 기본 폰트로
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();

	// BackEnd SetUp
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());
	
	// Sample Window
	// CIMGUITestWindow* window = AddWindow<CIMGUITestWindow>("TestWindow");

	// Font
	AddFont("Default", "NotoSansKR-Regular.otf", 15.f, true);
	AddFont("DefaultBlack", "NotoSansKR-Black.otf", 15.f, true);
	AddFont("DefaultBold", "NotoSansKR-Bold.otf", 15.f, true);
	
	mCurrentFont = FindFont("Default");
	return true;
}

void CIMGUIManager::Update(float deltaTime)
{
 //	// 윈도우 없으면 실행 안함
	if (mMapWindow.empty())
	{
		return;
	}

	static bool bOpen = false;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	// TODO : 폰트 기본 임시 설정
	// Font Push
	//ImGui::PushFont(mCurrentFont);

	auto iter = mMapWindow.begin();
	auto iterEnd = mMapWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update(deltaTime);
	}
	
	// TODO : 폰트 기본 임시 설정
	// Font Pop
	//ImGui::PopFont();

	// Window에서 그려낸 위젯들은 실제 화면에 그려지는 것이 아니라
	// IMGUI내부의 버퍼에 그려지게 된다.
	// 이 버퍼를 최종적으로 한 번 그려내는 방식으로 처리가 된다.
	ImGui::Render();
 }

void CIMGUIManager::Render()
{
	// DEMO
	// Update 주석처리 해야 함
 //	static bool bOpen = true;
 //	ImGui_ImplDX11_NewFrame();
 //	ImGui_ImplWin32_NewFrame();
 //	ImGui::NewFrame();
 //	ImGui::ShowDemoWindow(&bOpen);
 //	ImGui::Render();
 //	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
 //
 //	ImGuiIO& io = ImGui::GetIO();
 //	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
 //	{
 //		ImGui::UpdatePlatformWindows();
 //		ImGui::RenderPlatformWindowsDefault();
 //	}
 //
	if (mMapWindow.empty())
	{
		return;
	}

	// IMGUI 백버퍼 내에 그려져 있는 데이터들을 다시 그린다
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

bool CIMGUIManager::AddFont(const std::string& fontName, const char* fileName, const float size, const bool bKorean, const int overH, const int overV, const float spacing, const std::string& pathName)
{
	ImFont* font = FindFont(fontName);

	if (font)
	{
		return false;
	}

	char fullPath[MAX_PATH];

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}

	strcat_s(fullPath, fileName);

	return AddFontFullPath(fontName, fullPath, size, bKorean, overH, overV, spacing);
}

bool CIMGUIManager::AddFontFullPath(const std::string& fontName, const char* fullPath, const float size, const bool bKorean, const int overH, const int overV, const float spacing)
{
	ImFont* font = FindFont(fontName);

	if (font)
	{
		return false;
	}

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig config;

	// texture smpling 하듯 sampling quality 결정 -> H = 2, V = 1 최적화
	config.OversampleH = overH;
	config.OversampleV = overV;
	config.GlyphExtraSpacing.x = spacing;
	config.PixelSnapH = 1;

	if (!bKorean)
	{
		font = io.Fonts->AddFontFromFileTTF(fullPath, size, &config, io.Fonts->GetGlyphRangesDefault());
	}
	else
	{
		font = io.Fonts->AddFontFromFileTTF(fullPath, size, &config, io.Fonts->GetGlyphRangesKorean());
	}

	mMapFont.insert(std::make_pair(fontName, font));
	return true;
}

void CIMGUIManager::SetCurrentFont(const std::string& fontName)
{
	ImFont* font = FindFont(fontName);

	if (!font)
	{
		assert(false);
		return;
	}

	mCurrentFont = font;
}

ImFont* CIMGUIManager::FindFont(const std::string& fontName)
{
	auto iter = mMapFont.find(fontName);

	if (iter == mMapFont.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CIMGUIManager::PushCurrentFont()
{
	ImGui::PushFont(mCurrentFont);
}

void CIMGUIManager::PopCurrentFont()
{
	ImGui::PopFont();
}
