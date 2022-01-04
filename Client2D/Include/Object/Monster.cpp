#include "Monster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "MonsterAnimation.h"

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
	mBody = CreateComponent<CColliderBox2D>("Body");
	mBody->SetCollisionProfile("Monster");

	SetRootSceneComponent(mSprite);
	mSprite->AddChild(mBody);

	mSprite->CreateAnimationInstance<CMonsterAnimation>();
	
	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetRelativePos(500.f, 300.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);
	
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
