#include "PlayerObject.h"
#include "Input.h"

CPlayerObject::CPlayerObject()
{
	SetTypeID<CPlayerObject>();
}

CPlayerObject::CPlayerObject(const CPlayerObject& obj)
{
}

CPlayerObject::~CPlayerObject()
{
}

bool CPlayerObject::Init()
{
	CGameObject::Init();

	// Sprite
	mSprite = CreateComponent<CSpriteComponent>("Sprite");
	SetRootSceneComponent(mSprite);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	// Animation
	mSprite->LoadAnimationInstance("Player.anim");
	mSprite->GetAnimationInstance()->Play();
	mSprite->SetCurrentAnimation("TownIdle0");

	// Collider
	mBody = CreateComponent<CColliderBox2D>("Body");
	mBody->SetCollisionProfile("Player");
	mSprite->AddChild(mBody);

	// Camera
	mCamera = CreateComponent<CCameraComponent>("Camera");
	mCamera->OnViewportCenter();
	mSprite->AddChild(mCamera);

	// Key

	return true;
}

void CPlayerObject::Start()
{
	CGameObject::Start();
}

void CPlayerObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CPlayerObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CPlayerObject* CPlayerObject::Clone()
{
	return new CPlayerObject(*this);
}
