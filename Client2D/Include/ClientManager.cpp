#include "ClientManager.h"
#include "resource.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene./MainScene.h"
#include "Input.h"

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
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"), 1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CInput::GetInst()->CreateKey("MoveUp", 'W');
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');
	CInput::GetInst()->CreateKey("Move", VK_LBUTTON);
	CInput::GetInst()->CreateKey("Attack", VK_RBUTTON);

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CMainScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}
