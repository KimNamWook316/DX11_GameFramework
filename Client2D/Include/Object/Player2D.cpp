#include "Player2D.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "BulletCamera.h"
#include "../Widget/SimpleHUD.h"
#include "MuzzleParticle.h"
#include "BubbleParticle.h"

CPlayer2D::CPlayer2D()
{
	SetTypeID<CPlayer2D>();
	mOpacity = 1.f;
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj)	:
	CGameObject(obj)
{
}

CPlayer2D::~CPlayer2D()
{
}

bool CPlayer2D::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	mChildLeftSprite = CreateComponent<CSpriteComponent>("PlayerChildLeftSprite");
	mChildRightSprite = CreateComponent<CSpriteComponent>("PlayerChildRightSprite");
	mChildRoot = CreateComponent<CSceneComponent>("PlayerChildRoot");
	mMuzzle = CreateComponent<CSceneComponent>("Muzzle");
	mBody = CreateComponent<CColliderBox2D>("Body");

	mChildLeftMuzzle = CreateComponent<CSceneComponent>("LeftMuzzle");
	mChildRightMuzzle = CreateComponent<CSceneComponent>("RightMuzzle");

	mChild1Sprite = CreateComponent<CSpriteComponent>("PlayerChild1Sprite");
	mChild2Sprite = CreateComponent<CSpriteComponent>("PlayerChild2Sprite");
	mChild3Sprite = CreateComponent<CSpriteComponent>("PlayerChild3Sprite");
	mChild4Sprite = CreateComponent<CSpriteComponent>("PlayerChild4Sprite");

	mCamera = CreateComponent<CCameraComponent>("Camera");

	mSimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	mSimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");

	SetRootSceneComponent(mSprite);

	mBody->SetCollisionProfile("Player");

	mCamera->OnViewportCenter();
	//mCamera->SetViewportRatio(0.7f, 0.7f);

	//mSprite->GetMaterial()->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "MainTexture", )

	mSprite->AddChild(mBody);
	mSprite->AddChild(mCamera);
	mSprite->AddChild(mChildLeftSprite);
	mSprite->AddChild(mChildRightSprite);
	mSprite->AddChild(mMuzzle);
	mSprite->AddChild(mChildRoot);
	mSprite->AddChild(mSimpleHUDWidget);

	mSimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	mSprite->SetTransparency(true);
	//mSprite->SetOpacity(0.5f);

	mSprite->CreateAnimationInstance<CPlayerAnimation2D>();


	mChildLeftSprite->AddChild(mChildLeftMuzzle);
	mChildRightSprite->AddChild(mChildRightMuzzle);

	mChildLeftSprite->SetTexture(0, 0, (int)eBufferShaderTypeFlags::Pixel, "Teemo", TEXT("Teemo.jpg"));
	mChildRightSprite->SetTexture(0, 0, (int)eBufferShaderTypeFlags::Pixel, "Teemo", TEXT("Teemo.jpg"));

	mChildLeftSprite->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	mChildRightSprite->SetBaseColor(1.f, 0.f, 0.f, 1.f);


	mChildRoot->AddChild(mChild1Sprite);
	mChildRoot->AddChild(mChild2Sprite);
	mChildRoot->AddChild(mChild3Sprite);
	mChildRoot->AddChild(mChild4Sprite);

	mMuzzle->SetRelativePos(0.f, 150.f, 0.f);
	mMuzzle->SetBoolInheritRotZ(true);

	mChildLeftMuzzle->SetRelativePos(0.f, 100.f, 0.f);
	mChildLeftMuzzle->SetBoolInheritRotZ(true);

	mChildRightMuzzle->SetRelativePos(0.f, 100.f, 0.f);
	mChildRightMuzzle->SetBoolInheritRotZ(true);

	mChildRightSprite->SetRelativeScale(50.f, 50.f, 1.f);
	mChildRightSprite->SetBoolInheritScale(false);
	mChildRightSprite->SetRelativePos(100.f, 0.f, 0.f);
	mChildRightSprite->SetPivot(0.5f, 0.5f, 0.f);
	mChildRightSprite->SetBoolInheritRotZ(true);

	mChildLeftSprite->SetRelativeScale(50.f, 50.f, 1.f);
	mChildLeftSprite->SetBoolInheritScale(false);
	mChildLeftSprite->SetRelativePos(-100.f, 0.f, 0.f);
	mChildLeftSprite->SetPivot(0.5f, 0.5f, 0.f);
	mChildLeftSprite->SetBoolInheritRotZ(true);

	mChild1Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	mChild1Sprite->SetBoolInheritScale(false);
	mChild1Sprite->SetRelativePos(200.f, 0.f, 0.f);
	mChild1Sprite->SetPivot(0.5f, 0.5f, 0.f);
	mChild1Sprite->SetBoolInheritRotZ(true);

	mChild2Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	mChild2Sprite->SetBoolInheritScale(false);
	mChild2Sprite->SetRelativePos(-200.f, 0.f, 0.f);
	mChild2Sprite->SetPivot(0.5f, 0.5f, 0.f);
	mChild2Sprite->SetBoolInheritRotZ(true);

	mChild3Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	mChild3Sprite->SetBoolInheritScale(false);
	mChild3Sprite->SetRelativePos(0.f, 200.f, 0.f);
	mChild3Sprite->SetPivot(0.5f, 0.5f, 0.f);
	mChild3Sprite->SetBoolInheritRotZ(true);

	mChild4Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	mChild4Sprite->SetBoolInheritScale(false);
	mChild4Sprite->SetRelativePos(0.f, -200.f, 0.f);
	mChild4Sprite->SetPivot(0.5f, 0.5f, 0.f);
	mChild4Sprite->SetBoolInheritRotZ(true);

	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetRelativePos(100.f, 50.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::moveUp);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::moveDown);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::rotationZInv);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::rotationZ);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Attack", KeyState_Down, this, &CPlayer2D::attack);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Attack1", KeyState_Push, this, &CPlayer2D::attack1);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Skill1", KeyState_Down, this, &CPlayer2D::skill1);

	return true;
}

void CPlayer2D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	static bool	Hide = false;

	mChildRoot->AddRelativeRot(0.f, 0.f, 180.f * deltaTime);

	if (Hide)
	{
		mOpacity -= deltaTime / 5.f;

		if (mOpacity < 0.f)
			mOpacity = 0.f;

		mSprite->SetOpacity(mOpacity);
	}
}

void CPlayer2D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CGameObject* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::moveUp(float deltaTime)
{
	mSprite->AddRelativePos(mSprite->GetWorldAxis(eAXIS::AXIS_Y) * 300.f * deltaTime);
}

void CPlayer2D::moveDown(float deltaTime)
{
	mSprite->AddRelativePos(mSprite->GetWorldAxis(eAXIS::AXIS_Y) * -300.f * deltaTime);
}

void CPlayer2D::attack(float deltaTime)
{
	CBullet* Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(mMuzzle->GetWorldPos());
	Bullet->SetWorldRot(GetWorldRot());
	Bullet->SetCollisionProfile("PlayerAttack");

	Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(mChildLeftMuzzle->GetWorldPos());
	Bullet->SetWorldRot(GetWorldRot());
	Bullet->SetCollisionProfile("PlayerAttack");

	Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(mChildRightMuzzle->GetWorldPos());
	Bullet->SetWorldRot(GetWorldRot());
	Bullet->SetCollisionProfile("PlayerAttack");

	CBubbleParticle* BubbleParticle = mScene->CreateGameObject<CBubbleParticle>("BubbleParticle");

	BubbleParticle->SetRelativePos(mChildRightMuzzle->GetWorldPos());
	BubbleParticle->SetLifeSpan(5.f);
}

void CPlayer2D::attack1(float deltaTime)
{
	CBullet* Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	Bullet->SetWorldPos(mMuzzle->GetWorldPos());
	Bullet->SetWorldRot(GetWorldRot());
	Bullet->SetCollisionProfile("PlayerAttack");

	Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	Bullet->SetWorldPos(mMuzzle->GetWorldPos());
	Bullet->SetWorldRot(GetWorldRot() + Vector3(0.f, 0.f, 45.f));
	Bullet->SetCollisionProfile("PlayerAttack");

	Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	Bullet->SetWorldPos(mMuzzle->GetWorldPos());
	Bullet->SetWorldRot(GetWorldRot() + Vector3(0.f, 0.f, -45.f));
	Bullet->SetCollisionProfile("PlayerAttack");
}

void CPlayer2D::skill1(float deltaTime)
{
 //	CBulletCamera* Bullet = mScene->CreateGameObject<CBulletCamera>("Bullet");
 //
 //	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
 //	Bullet->SetWorldPos(mMuzzle->GetWorldPos());
 //	Bullet->SetWorldRot(GetWorldRot());
 //	Bullet->SetCollisionProfile("PlayerAttack");
 }

void CPlayer2D::rotationZInv(float deltaTime)
{
	mSprite->AddWorldRotZ(180.f * deltaTime);
}

void CPlayer2D::rotationZ(float deltaTime)
{
	mSprite->AddWorldRotZ(-180.f * deltaTime);
}

