#include "ClientManager.h"
#include "resource.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/MainScene.h"
#include "Scene/StartScene.h"
#include "Input.h"
#include "Resource/ResourceManager.h"
#include "Widget/MouseNormal.h"
#include "Widget/MouseAttack.h"

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

 //	CSceneManager::GetInst()->SetCreateSceneModeCallBack<CClientManager>(this, &CClientManager::CreateSceneMode);
 //	CSceneManager::GetInst()->SetCreateObjectCallBack<CClientManager>(this, &CClientManager::CreateObject);

	CInput::GetInst()->CreateKey("MoveUp", 'W');
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');
	CInput::GetInst()->CreateKey("Move", VK_LBUTTON);
	CInput::GetInst()->CreateKey("Attack", VK_RBUTTON);
	CInput::GetInst()->CreateKey("Skill1", '1');

	CResourceManager::GetInst()->CreateSoundChannelGroup("UI");

	// 마우스 위젯 설정
	CMouseNormal* mouseNormal = CEngine::GetInst()->CreateMouse<CMouseNormal>(eMouseState::Normal, "MouseNormal");
	CMouseAttack* mouseAttack = CEngine::GetInst()->CreateMouse<CMouseAttack>(eMouseState::State1, "MouseAttack");

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CStartScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* scene, size_t type)
{
}

void CClientManager::CreateObject(CScene* scene, size_t type)
{
}
