#include "SceneManager.h"
#include "../Render/RenderManager.h"

DEFINITION_SINGLE(CSceneManager);

CSceneManager::CSceneManager()
	: mScene(nullptr)
	, mNextScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(mScene);
	SAFE_DELETE(mNextScene);
}

bool CSceneManager::Init()
{
	mScene = new CScene;
	
	// 렌더 매니저에 자신이 들고 있는 오브젝트 리스트의 포인터를 전달한다.
	// 포인터를 들고 있기 때문에, 오브젝트 생성, 삭제할 때 렌더 매니저에 일일히 전달할 필요 없다.
	CRenderManager::GetInst()->SetObjList(&mScene->mObjList);
	
	return true;
}

bool CSceneManager::Update(float deltaTime)
{
	mScene->Update(deltaTime);
	return true;
}

bool CSceneManager::PostUpdate(float deltaTime)
{
	mScene->PostUpdate(deltaTime);
	return true;
}
