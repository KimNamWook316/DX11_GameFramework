#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "../Object/PlayerChild.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CPlayer2D* player = mScene->CreateGameObject<CPlayer2D>("Player");

	CPlayerChild* child1 = mScene->CreateGameObject<CPlayerChild>("PlayerChild1");
	CPlayerChild* child2 = mScene->CreateGameObject<CPlayerChild>("PlayerChild2");
	CPlayerChild* child3 = mScene->CreateGameObject<CPlayerChild>("PlayerChild3");
	CPlayerChild* child4 = mScene->CreateGameObject<CPlayerChild>("PlayerChild4");
	child2->SetWorldPos(1000.f, 200.f, 0.f);
	child3->SetWorldPos(1200.f, 300.f, 0.f);
	child4->SetWorldPos(500.f, 600.f, 0.f);


	SetPlayerObj(player);

	return true;
}
