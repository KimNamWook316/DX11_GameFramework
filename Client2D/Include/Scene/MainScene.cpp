#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "../Object/Monster.h"
#include "Scene/SceneResource.h";

CMainScene::CMainScene()
{
	SetTypeID<CMainScene>();
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	createMaterial();
	createAnimationSequence();

	CPlayer2D* player = mScene->CreateGameObject<CPlayer2D>("Player");

	CMonster* monster = mScene->CreateGameObject<CMonster>("Monster");

	SetPlayerObj(player);

	return true;
}

void CMainScene::createMaterial()
{
}

void CMainScene::createAnimationSequence()
{
	mScene->GetResource()->CreateAnimationSequence2D("PlayerIdle_Front", 
		"PlayerIdle", TEXT("Sorceress_Town_Idle.png"));
	mScene->GetResource()->CreateAnimationSequence2D("PlayerIdle_Back", 
		"PlayerIdle", TEXT("Sorceress_Town_Idle.png"));
	mScene->GetResource()->CreateAnimationSequence2D("PlayerMove_Front", 
		"PlayerMove", TEXT("Sorceress_Town_Move.png"));
	mScene->GetResource()->CreateAnimationSequence2D("PlayerMove_Back", 
		"PlayerMove", TEXT("Sorceress_Town_Move.png"));
	mScene->GetResource()->CreateAnimationSequence2D("PlayerMove_Right", 
		"PlayerMove", TEXT("Sorceress_Town_Move.png"));
	mScene->GetResource()->CreateAnimationSequence2D("PlayerMove_Left", 
		"PlayerMove", TEXT("Sorceress_Town_Move.png"));
	mScene->GetResource()->CreateAnimationSequence2D("PlayerAttack", 
		"PlayerAttack", TEXT("Sorceress_Special_Attack_1.png"));
	
	for (int i = 0; i < 6; ++i)
	{
		mScene->GetResource()->AddAnimationSequece2DFrame("PlayerIdle_Front", Vector2(i * 42.f, 0.f), Vector2(42.f, 73.f));
	}

	for (int i = 0; i < 6; ++i)
	{
		mScene->GetResource()->AddAnimationSequece2DFrame("PlayerIdle_Back", Vector2(i * 42.f, 0.f), Vector2(42.f, 73.f));
	}

	for (int i = 0; i < 8; ++i)
	{
		mScene->GetResource()->AddAnimationSequece2DFrame("PlayerMove_Front", Vector2(i * 48.f, 0.f), Vector2(48.f, 75.f));
	}

	for (int i = 0; i < 8; ++i)
	{
		mScene->GetResource()->AddAnimationSequece2DFrame("PlayerMove_Back", Vector2(i * 48.f, 600.f), Vector2(48.f, 75.f));
	}

	for (int i = 0; i < 14; ++i)
	{
		mScene->GetResource()->AddAnimationSequece2DFrame("PlayerAttack", Vector2(i * 89.f, 0.f), Vector2(89.f, 91.f));
	}

} 