#include "ClientManager.h"
#include "Engine.h"
#include "Scene/TestScene.h"
#include "Scene/SceneManager.h"
#include "resource.h"

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("Diablo2"), 800, 600, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}
	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CTestScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}
