#include "TestScene.h"
#include "Scene/Scene.h"
#include "../Object/D2Player.h"

CTestScene::CTestScene()
{
	SetTypeID<CTestScene>();
}

CTestScene::~CTestScene()
{
}

bool CTestScene::Init()
{
	CD2Player* player = mScene->CreateGameObject<CD2Player>("Player");
	SetPlayerObj(player);
	return true;
}
