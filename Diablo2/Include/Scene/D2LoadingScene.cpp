#include "D2LoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "D2LoadingThread.h"
#include "Scene/SceneManager.h"
#include "Resource/Excel/ExcelData.h"
#include "Resource/ResourceManager.h"

CD2LoadingScene::CD2LoadingScene()	:
	mLoadingThread(nullptr)
{
	SetTypeID<CD2LoadingScene>();
}

CD2LoadingScene::~CD2LoadingScene()
{
	SAFE_DELETE(mLoadingThread)
}

bool CD2LoadingScene::Init()
{
	loadWindow();

	mLoadingThread = CThread::CreateThread<CD2LoadingThread>("D2LoadingThread");
	mLoadingThread->Start();

	return true;
}

void CD2LoadingScene::Update(float deltaTime)
{
	CSceneMode::Update(deltaTime);

	CThreadQueue<D2LoadingMessage>* queue = mLoadingThread->GetLoadingQueue();
	if (!queue->Empty())
	{
		D2LoadingMessage msg = queue->Front();
		queue->Pop();
		mLoadingWindow->SetLoadingPercent(msg.Percent);

		if (msg.bComplete)
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}
}

bool CD2LoadingScene::loadWindow()
{
	std::string name;
	CResourceManager::GetInst()->LoadCSV(name, "D2WindowInfo.csv");

	CExcelData* csv = CResourceManager::GetInst()->FindCSV(name);

	std::vector<std::string>* row = csv->GetRow("LoadingScreen");
	mLoadingWindow = mScene->GetViewport()->CreateWidgetWindow<CD2LoadingWindow>("LoadingWindow");
	Vector2 size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	Vector2 pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mLoadingWindow->SetSize(size);
	mLoadingWindow->SetPos(pos);

	CResourceManager::GetInst()->LoadCSV(name, "D2WidgetInfo.csv");

	CExcelData* widgetCSV = CResourceManager::GetInst()->FindCSV(name);
	mLoadingWindow->MakeWidget(widgetCSV);
	return true;
}
