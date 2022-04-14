#include "D2LoadingThread.h"
#include "D2MainScene.h"
#include "Scene/SceneManager.h"

CD2LoadingThread::CD2LoadingThread()	:
	mQueue{}
{
}

CD2LoadingThread::~CD2LoadingThread()
{
}

bool CD2LoadingThread::Init()
{
	if (!CThread::Init())
	{
		assert(false);
		return false;
	}

	return true;
}

void CD2LoadingThread::Run()
{
	CSceneManager::GetInst()->CreateNextScene(false);
	CD2MainScene* mainScene = CSceneManager::GetInst()->CreateSceneModeNotInitilized<CD2MainScene>(false);
	mainScene->SetLoadingFunction(this, &CD2LoadingThread::AddMessage);
	mainScene->Init();
	AddMessage(false, 1.f);
	Sleep(2000);
	AddMessage(true, 1.f);

	mainScene->OnLoadEnd();
}
