#include "MainScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "../Object/Player2D.h"
#include "../Object/Monster.h"
#include "../Object/PixelTest.h"
#include "../Object/BubbleParticle.h"
#include "../Object/SmokeParticle.h"
#include "../Object/MuzzleParticle.h"
#include "../Object/RainParticle.h"
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
	createParticle();

	if (mLoadingFunction)
	{
		mLoadingFunction(false, 0.3f);
	}

	CPlayer2D* player = mScene->CreateGameObject<CPlayer2D>("Player");

	//CMonster* monster = mScene->CreateGameObject<CMonster>("Monster");

	if (mLoadingFunction)
	{
		mLoadingFunction(false, 0.6f);
	}

	//CPixelTest* pixelTest = mScene->CreateGameObject<CPixelTest>("PixelTest");

	//mMainWidget = mScene->GetViewport()->CreateWidgetWindow<CMainWidget>("MainWidget");

	SetPlayerObj(player);

	if (mLoadingFunction)
	{
		mLoadingFunction(false, 0.8f);
	}

	//CBubbleParticle* bubbleParticle = mScene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	//bubbleParticle->SetRelativePos(200.f, 200.f, 0.f);

	CSmokeParticle* smokeParticle = mScene->CreateGameObject<CSmokeParticle>("SmokeParticle");
	smokeParticle->SetRelativePos(400.f, 200.f, 0.f);

 //	CMuzzleParticle* muzzleParticle = mScene->CreateGameObject<CMuzzleParticle>("MuzzleParticle");
 //	muzzleParticle->SetRelativePos(100.f, 200.f, 0.f);

	CRainParticle* rainParticle = mScene->CreateGameObject<CRainParticle>("RainParticle");
	rainParticle->SetRelativePos(640.f, 0.f, 0.f);

	return true;
}

void CMainScene::createMaterial()
{
	mScene->GetResource()->CreateMaterial<CMaterial>("Bubble");
	CMaterial* material = mScene->GetResource()->FindMaterial("Bubble");

	material->AddTexture(0, (int)eBufferShaderTypeFlags::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));
	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");
	material->SetTransparency(true);

	mScene->GetResource()->CreateMaterial<CMaterial>("Smoke");
	material = mScene->GetResource()->FindMaterial("Smoke");
	material->AddTexture(0, (int)eBufferShaderTypeFlags::Pixel, "Smoke", TEXT("Particle/Smoke2.png"));
	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");
	material->SetTransparency(true);

	mScene->GetResource()->CreateMaterial<CMaterial>("Flash");
	material = mScene->GetResource()->FindMaterial("Flash");
	material->AddTexture(0, (int)eBufferShaderTypeFlags::Pixel, "Flash", TEXT("Particle/muzzle.png"));
	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");
	material->SetTransparency(true);

	mScene->GetResource()->CreateMaterial<CMaterial>("Fragment");
	material = mScene->GetResource()->FindMaterial("Fragment");
	material->AddTexture(0, (int)eBufferShaderTypeFlags::Pixel, "Fragment", TEXT("Particle/HardCircle.png"));
	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");
	material->SetTransparency(true);

	mScene->GetResource()->CreateMaterial<CMaterial>("Rain");
	material = mScene->GetResource()->FindMaterial("Rain");
	material->AddTexture(0, (int)eBufferShaderTypeFlags::Pixel, "Rain", TEXT("Particle/HardRain.png"));
	material->SetShader("ParticleRenderShader");
	material->SetRenderState("AlphaBlend");
	material->SetTransparency(true);
}

void CMainScene::createAnimationSequence()
{
 	mScene->GetResource()->LoadSequence2D("PlayerIdle_Front.sqc");
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

void CMainScene::createParticle()
{
	mScene->GetResource()->CreateParticle("Bubble");
	CParticle* particle = mScene->GetResource()->FindParticle("Bubble");

	CMaterial* material = mScene->GetResource()->FindMaterial("Bubble");

	particle->SetMaterial(material);
	particle->SetSpawnCountMax(1000);
	particle->SetLifeTimeMin(3.f);
	particle->SetLifeTimeMax(5.f);
	particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	particle->SetSpeedMin(100.f);
	particle->SetSpeedMax(300.f);
	particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
	particle->SetStartMax(Vector3(30.f, 30.f, 0.f));
	particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
	particle->SetColorMax(Vector4(0.2f, 0.1f, 0.8f, 1.f));
	particle->SetMoveAngle(Vector3(0.f, 0.f, 90.f));
	particle->SetIsGravity(true);
	particle->SetIsMove(true);

	mScene->GetResource()->CreateParticle("Smoke");
	particle = mScene->GetResource()->FindParticle("Smoke");
	material = mScene->GetResource()->FindMaterial("Smoke");

	particle->SetMaterial(material);
	particle->SetSpawnTime(0.001f);
	particle->SetSpawnCountMax(1500);
	particle->SetLifeTimeMin(10.f);
	particle->SetLifeTimeMax(15.f);
	particle->SetScaleMin(Vector3(100.f, 100.f, 1.f));
	particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	particle->SetSpeedMin(30.f);
	particle->SetSpeedMax(50.f);
	particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	particle->SetStartMin(Vector3(-100.f, -100.f, 0.f));
	particle->SetStartMax(Vector3(100.f, 100.f, 0.f));
	particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
	particle->SetColorMax(Vector4(0.5f, 0.5f, 0.5f, 0.1f));
	particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));
	particle->SetIsMove(true);

	mScene->GetResource()->CreateParticle("Flash");
	particle = mScene->GetResource()->FindParticle("Flash");
	material = mScene->GetResource()->FindMaterial("Flash");

	particle->SetMaterial(material);
	particle->SetSpawnCountMax(1);
	particle->SetLifeTimeMin(0.1f);
	particle->SetLifeTimeMax(0.1f);
	particle->SetScaleMin(Vector3(100.f, 100.f, 1.f));
	particle->SetScaleMax(Vector3(100.f, 100.f, 1.f));
	particle->SetSpeedMin(0.f);
	particle->SetSpeedMax(0.f);
	particle->SetMoveDir(Vector3(1.f, 0.f, 0.f));
	particle->SetStartMin(Vector3(0.f, 0.f, 0.f));
	particle->SetStartMax(Vector3(0.f, 0.f, 0.f));
	particle->SetColorMin(Vector4(1.f, 0.8f, 0.8f, 1.f));
	particle->SetColorMax(Vector4(0.5f, 0.f, 0.f, 0.1f));
	particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));

	mScene->GetResource()->CreateParticle("Fragment");
	particle = mScene->GetResource()->FindParticle("Fragment");
	material = mScene->GetResource()->FindMaterial("Fragment");

	particle->SetMaterial(material);
	particle->SetSpawnCountMax(100);
	particle->SetSpawnTime(0.001f);
	particle->SetLifeTimeMin(0.1f);
	particle->SetLifeTimeMax(0.2f);
	particle->SetScaleMin(Vector3(5.f, 5.f, 1.f));
	particle->SetScaleMax(Vector3(8.f, 8.f, 1.f));
	particle->SetSpeedMin(1000.f);
	particle->SetSpeedMax(500.f);
	particle->SetMoveDir(Vector3(1.f, 0.f, 0.f));
	particle->SetStartMin(Vector3(0.f, -10.f, 0.f));
	particle->SetStartMax(Vector3(0.f, 10.f, 0.f));
	particle->SetColorMin(Vector4(1.f, 0.8f, 0.8f, 1.f));
	particle->SetColorMax(Vector4(1.f, 0.8f, 0.8f, 0.1f));
	particle->SetMoveAngle(Vector3(0.f, 0.f, 90.f));
	particle->SetIsMove(true);

	mScene->GetResource()->CreateParticle("Rain");
	particle = mScene->GetResource()->FindParticle("Rain");
	material = mScene->GetResource()->FindMaterial("Rain");

	particle->SetMaterial(material);
	particle->SetSpawnTime(0.01f);
	particle->SetSpawnCountMax(1000);
	particle->SetLifeTimeMin(5.f);
	particle->SetLifeTimeMax(10.f);
	particle->SetScaleMin(Vector3(100.f, 100.f, 1.f));
	particle->SetScaleMax(Vector3(100.f, 100.f, 1.f));
	particle->SetSpeedMin(30.f);
	particle->SetSpeedMax(60.f);
	particle->SetMoveDir(Vector3(0.3f, -0.7f, 0.f));
	particle->SetStartMin(Vector3(-1000.f, 0.f, 0.f));
	particle->SetStartMax(Vector3(1000.f, 0.f, 0.f));
	particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
	particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.5f));
	particle->SetIsMove(true);
	particle->SetIsGravity(true);
}
