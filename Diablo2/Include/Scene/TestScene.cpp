#include "TestScene.h"
#include "Scene/Scene.h"
#include "../Object/PlayerObject.h"

CTestScene::CTestScene()
{
	SetTypeID<CTestScene>();
}

CTestScene::~CTestScene()
{
}

bool CTestScene::Init()
{
	CPlayerObject* plyaer = mScene->CreateGameObject<CPlayerObject>("Player");
	SetPlayerObj(plyaer);
	return true;
}
