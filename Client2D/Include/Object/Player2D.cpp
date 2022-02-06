#include "Player2D.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "BulletCamera.h"
#include "../Widget/SimpleHUD.h"
#include "MuzzleParticle.h"

CPlayer2D::CPlayer2D()	:
	mOpacity(1.f),
	mbIsMoving(false)
{
	SetTypeID<CPlayer2D>();
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
	mBody = CreateComponent<CColliderBox2D>("Body");
	mCamera = CreateComponent<CCameraComponent>("Camera");
	mSimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	mSimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");

	mSprite->CreateAnimationInstance<CPlayerAnimation2D>();
	mSprite->LoadAnimationInstance("PlayerTownIdle.anim");
	mSprite->AddChild(mBody);
	mSprite->AddChild(mCamera);
	mSprite->AddChild(mSimpleHUDWidget);

	SetRootSceneComponent(mSprite);

	mSprite->SetWorldPos(0.f, 0.f, 0.f);
	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::moveUp);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::moveDown);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Attack", KeyState_Down, this, &CPlayer2D::playAttackAnim);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Move", KeyState_Down, this, &CPlayer2D::move);

	CInput::GetInst()->CreateKey("muzzle", VK_SPACE);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("muzzle", KeyState_Down, this, &CPlayer2D::Fire);


 //	mSprite->SetEndCallBack<CPlayer2D>("Attack", this, &CPlayer2D::skill1);
 //	mSprite->SetPlayTime("Attack", 0.5f);

	mBody->SetCollisionProfile("Player");

	mCamera->OnViewportCenter();
	mCamera->SetWorldScale(1.f, 1.f, 1.f);
	//mCamera->SetViewPortByRatio(3.f, 3.f);

	mSimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	return true;
}

void CPlayer2D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	static bool bFire = false;

	static bool bHide = false;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		bHide = true;
	}

	if (bHide)
	{
		mOpacity -= deltaTime;

		if (mOpacity < 0.f)
		{
			mOpacity = 0.f;
		}

		mSprite->SetOpacity(mOpacity);
	}

	if (mbIsMoving)
	{
		if ((GetWorldPos() - mMovePosition).Length() < mSpeed * deltaTime)
		{
			mbIsMoving = false;
			mSpeed = 0.f;
			//mSprite->ChangeAnimation("Idle_Front");
		}
		else
		{
			AddWorldPos(mDirection * mSpeed * deltaTime);
		}
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
void CPlayer2D::move(float deltaTime)
{
	mMovePosition = Vector3(CInput::GetInst()->GetMouseWorld2DPos().x, CInput::GetInst()->GetMouseWorld2DPos().y, 0.f);
	mDirection = mMovePosition - GetWorldPos();
	mDirection.Normalize();
	mSpeed = 300.f;
	mbIsMoving = true;

 //	if (mDirection.y > 0)
 //	{
 //		mSprite->ChangeAnimation("Move_Back");
 //	}
 //	else
 //	{
 //		mSprite->ChangeAnimation("Move_Front");
 //	}
}

void CPlayer2D::moveUp(float deltaTime)
{
	mSprite->AddRelativePos(mSprite->GetWorldAxis(eAXIS::AXIS_Y) * 300.f * deltaTime);
}

void CPlayer2D::moveDown(float deltaTime)
{
	mSprite->AddRelativePos(mSprite->GetWorldAxis(eAXIS::AXIS_Y) * -300.f * deltaTime);
}

void CPlayer2D::rotationZInv(float deltaTime)
{
	mSprite->AddWorldRotZ(180.f * deltaTime);
}

void CPlayer2D::rotationZ(float deltaTime)
{
	mSprite->AddWorldRotZ(-180.f * deltaTime);
}

void CPlayer2D::attack()
{
	CBullet* bullet = mScene->CreateGameObject<CBullet>("Bullet");

	bullet->SetWorldPos(GetWorldPos());

	float rot = Vector3::Axis[eAXIS::AXIS_Y].Angle(mDirection);

	if (mDirection.x < 0)
	{
		rot *= -1;
	}

	bullet->SetWorldRotZ(-rot);
	mSprite->ChangeAnimation("Idle_Front");
}

void CPlayer2D::skill1()
{
	CBulletCamera* bullet = mScene->CreateGameObject<CBulletCamera>("BulletCam");

	bullet->SetWorldPos(GetWorldPos());

	float rot = Vector3::Axis[eAXIS::AXIS_Y].Angle(mDirection);

	if (mDirection.x < 0)
	{
		rot *= -1;
	}

	bullet->SetWorldRotZ(-rot);
	mSprite->ChangeAnimation("Idle_Front");
}

void CPlayer2D::Fire(float deltaTime)
{
	Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
	CMuzzleParticle* muzzleParticle = mScene->CreateGameObject<CMuzzleParticle>("MuzzleParticle");
	muzzleParticle->SetRelativePos(Vector3(mousePos.x, mousePos.y, 0.f));
}

void CPlayer2D::playAttackAnim(float deltaTime)
{
	mbIsMoving = false;
	mSpeed = 0.f;
	Vector3 mousePos = Vector3(CInput::GetInst()->GetMouseWorld2DPos().x, CInput::GetInst()->GetMouseWorld2DPos().y, 0.f);
	mDirection = mousePos - GetWorldPos();
	mDirection.Normalize();

	mSprite->ChangeAnimation("Attack");
}
