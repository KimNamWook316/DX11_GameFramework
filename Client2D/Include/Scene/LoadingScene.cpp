#include "LoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "LoadingThread.h"
#include "Scene/SceneManager.h"

CLoadingScene::CLoadingScene()
{
	SetTypeID<CLoadingScene>();
}

CLoadingScene::~CLoadingScene()
{
	SAFE_DELETE(mLoadingThread);
}

bool CLoadingScene::Init()
{
	mLoadingWidget = mScene->GetViewport()->CreateWidgetWindow<CLoadingWidget>("LoadingWidget");

	// 로딩 스레드 생성
	mLoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");
	mLoadingThread->Start();
	return true;
}

void CLoadingScene::Update(float deltaTime)
{
	CSceneMode::Update(deltaTime);
	CThreadQueue<LoadingMessage>* queue = mLoadingThread->GetLoadingQueue();
	if (!queue->Empty())
	{
		LoadingMessage msg = queue->Front();
		queue->Pop();
		mLoadingWidget->SetLoadingPercent(msg.Percent);
		if (msg.Complete)
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}
}
