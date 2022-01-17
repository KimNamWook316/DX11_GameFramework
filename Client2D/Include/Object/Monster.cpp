#include "Monster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "MonsterAnimation.h"
#include "../Widget/SimpleHUD.h"

CMonster::CMonster()
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& obj)	:
	CGameObject(obj)
{
	SetTypeID<CMonster>();

	mSprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	SetRootSceneComponent(mSprite);
	mBody = (CColliderBox2D*)FindComponent("Body");
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	mSimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	mSimpleHUD = mSimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");
	mBody = CreateComponent<CColliderBox2D>("Body");
	mBody->SetCollisionProfile("Monster");

	SetRootSceneComponent(mSprite);
	mSprite->AddChild(mBody);
	mSprite->AddChild(mSimpleHUDWidget);

	//mSprite->CreateAnimationInstance<CMonsterAnimation>();

	mSimpleHUD->SetText(TEXT("Teemo"));
	
	mSprite->SetRelativeScale(500.f, 500.f, 1.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);
	mSprite->SetWorldPos(300.f, 300.f, 0.f);
	mSimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);
	mSimpleHUD->SetHPDir(eProgressBarDir::RightToLeft);
	mSimpleHUD->SetHPPercent(0.5f);
	
	mBody->AddCollisionCallBack(eCollisionState::Enter, this, &CMonster::OnCollsionCallBack);

	mHP = 3;
	return true;
}

void CMonster::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CMonster::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::OnCollsionCallBack(const CollisionResult& result)
{
	AddHP(-1);
}
