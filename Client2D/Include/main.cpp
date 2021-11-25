#include "Engine.h"
#include "resource.h"
#include "Scene/MainScene.h";
#include "Scene/SceneManager.h"

#ifdef _DEBUG

#pragma comment(lib, "GameEngine_Debug.lib")

#else

#pragma comment(lib, "GameEngine.lib")

#endif // _DEBUG

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){

	if (!CEngine::GetInst()->Init(hInstance, TEXT("GameEngine"), 1280, 720, IDI_ICON1)) 
	{
		CEngine::DestroyInst();
		return 0;
	}
	
	CSceneManager::GetInst()->CreateSceneMode<CMainScene>();

	int Ret = CEngine::GetInst()->Run();

	CEngine::DestroyInst();
	
	return Ret;
}