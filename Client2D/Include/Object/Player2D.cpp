#include "Player2D.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"

CPlayer2D::CPlayer2D()	:
	mOpacity(1.f),
	mbIsMoving(false)
{
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

	SetRootSceneComponent(mSprite);

	SetRootSceneComponent(mSprite);

	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetWorldPos(200.f, 100.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::moveUp);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::moveDown);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Attack", KeyState_Down, this, &CPlayer2D::playAttackAnim);
	CInput::GetInst()->SetKeyCallBack<CPlayer2D>("Move", KeyState_Down, this, &CPlayer2D::move);

	mSprite->CreateAnimationInstance<CPlayerAnimation2D>();
	mSprite->SetEndCallBack<CPlayer2D>("Attack", this, &CPlayer2D::attack);
	mSprite->SetPlayTime("Attack", 0.5f);

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
			mSprite->ChangeAnimation("Idle_Front");
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
	mMovePosition = Vector3(CInput::GetInst()->GetMousePos().x, 720.f - CInput::GetInst()->GetMousePos().y, 0.f);
	mDirection = mMovePosition - GetWorldPos();
	mDirection.Normalize();
	mSpeed = 300.f;
	mbIsMoving = true;

	if (mDirection.y > 0)
	{
		mSprite->ChangeAnimation("Move_Back");
	}
	else
	{
		mSprite->ChangeAnimation("Move_Front");
	}
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
	CBullet* Bullet = mScene->CreateGameObject<CBullet>("Bullet");

	Bullet->SetWorldPos(GetWorldPos());

	float rot = Vector3::Axis[eAXIS::AXIS_Y].Angle(mDirection);

	if (mDirection.x < 0)
	{
		rot *= -1;
	}

	Bullet->SetWorldRotZ(-rot);
	mSprite->ChangeAnimation("Idle_Front");
}

void CPlayer2D::playAttackAnim(float deltaTime)
{
	mbIsMoving = false;
	mSpeed = 0.f;
	Vector3 mousePos = Vector3(CInput::GetInst()->GetMousePos().x, 720.f - CInput::GetInst()->GetMousePos().y, 0.f);
	mDirection = mousePos - GetWorldPos();
	mDirection.Normalize();

	mSprite->ChangeAnimation("Attack");
}
