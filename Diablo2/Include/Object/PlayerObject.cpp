#include "PlayerObject.h"
#include "Input.h"

CPlayerObject::CPlayerObject()	:
	mCharInfo{}
{
	SetTypeID<CPlayerObject>();
}

CPlayerObject::CPlayerObject(const CPlayerObject& obj)	:
	CGameObject(obj)
{
	mCharInfo = obj.mCharInfo;
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
	mSprite->SetPivot(0.5f, 0.f, 0.f);

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

	float width = mSprite->GetMaterial()->GetTexture()->GetWidth();
	float height = mSprite->GetMaterial()->GetTexture()->GetHeight();

	mSprite->SetWorldScale(width / 15.f, height / 15.f, 1.f);
	mBody->SetOffset(0.f, height / 30.f, 0.f);
	mBody->SetExtent(width / 30.f, height / 30.f);

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
