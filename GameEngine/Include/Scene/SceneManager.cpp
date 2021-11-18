#include "SceneManager.h"

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
