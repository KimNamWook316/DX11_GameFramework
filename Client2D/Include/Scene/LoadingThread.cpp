#include "LoadingThread.h"
#include "MainScene.h"
#include "Scene/SceneManager.h"

CLoadingThread::CLoadingThread()
{
}

CLoadingThread::~CLoadingThread()
{
}

bool CLoadingThread::Init()
{
	if (!CThread::Init())
	{
		assert(false);
		return false;
	}
	return true;
}

void CLoadingThread::Run()
{
	// Loading
	CSceneManager::GetInst()->CreateNextScene(false);
	CMainScene* mainScene = CSceneManager::GetInst()->CreateSceneModeNotInitilized<CMainScene>(false);
	mainScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);
	mainScene->Init();
	AddMessage(false, 1.f);
	Sleep(2000);
	AddMessage(true, 1.f);
}
