#include "SceneManager.h"
#include "../Render/RenderManager.h"
#include "../Sync.h"

DEFINITION_SINGLE(CSceneManager);

CSceneManager::CSceneManager()
	: mScene(nullptr)
	, mNextScene(nullptr)
{
	InitializeCriticalSection(&mCrt);
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(mScene);
	SAFE_DELETE(mNextScene);
	DeleteCriticalSection(&mCrt);
}

bool CSceneManager::Init()
{
	mScene = new CScene;
	
	// 렌더 매니저에 자신이 들고 있는 오브젝트 리스트의 포인터를 전달한다.
	// 포인터를 들고 있기 때문에, 오브젝트 생성, 삭제할 때 렌더 매니저에 일일히 전달할 필요 없다.
	CRenderManager::GetInst()->SetObjList(&mScene->mObjList);
	
	return true;
}

void CSceneManager::Start()
{
	mScene->Start();
}

bool CSceneManager::Update(float deltaTime)
{
	mScene->Update(deltaTime);
	return changeScene();
}

bool CSceneManager::PostUpdate(float deltaTime)
{
	mScene->PostUpdate(deltaTime);
	return changeScene();
}

void CSceneManager::CreateNextScene(bool bBeChangeNow)
{
	CSync sync(&mCrt);

	// 다른 스레드가 이 영역에 접근하려고 하면, 그 Thread를 대기 상태로 설정하고, 수행이 끝날 때까지 기다린다.
	SAFE_DELETE(mNextScene);

	mNextScene = new CScene;
	mNextScene->SetBeChange(bBeChangeNow);
}

void CSceneManager::ChangeNextScene()
{
	CSync sync(&mCrt);
	mNextScene->SetBeChange(true);
}

bool CSceneManager::changeScene()
{
	CSync sync(&mCrt);

	if (mNextScene)
	{
		if (mNextScene->mbBeChange)
		{
			SAFE_DELETE(mScene);
			mScene = mNextScene;
			mNextScene = nullptr;

			CRenderManager::GetInst()->SetObjList(&mScene->mObjList);
			mScene->Start();
			return true;
		}
	}
	return false;
}
