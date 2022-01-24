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
	
	// ���� �Ŵ����� �ڽ��� ��� �ִ� ������Ʈ ����Ʈ�� �����͸� �����Ѵ�.
	// �����͸� ��� �ֱ� ������, ������Ʈ ����, ������ �� ���� �Ŵ����� ������ ������ �ʿ� ����.
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

	// �ٸ� �����尡 �� ������ �����Ϸ��� �ϸ�, �� Thread�� ��� ���·� �����ϰ�, ������ ���� ������ ��ٸ���.
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
