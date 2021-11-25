#include "Player2D.h"
#include "Bullet.h"
#include "Scene/Scene.h"

CPlayer2D::CPlayer2D()
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
	mChildLeftSprite = CreateComponent<CSpriteComponent>("PlayerChildLeftSprite");
	mChildRightSprite = CreateComponent<CSpriteComponent>("PlayerChildRightSprite");
	mChildLeftMuzzle = CreateComponent<CSceneComponent>("LeftMuzzle");
	mChildRightMuzzle = CreateComponent<CSceneComponent>("LeftMuzzle");
	mChildRoot = CreateComponent<CSceneComponent>("PlayerChildRoot");
	mMuzzle = CreateComponent<CSceneComponent>("Muzzle");

	mChild1Sprite = CreateComponent<CSpriteComponent>("PlayerChild1Sprite");
	mChild2Sprite = CreateComponent<CSpriteComponent>("PlayerChild2Sprite");
	mChild3Sprite = CreateComponent<CSpriteComponent>("PlayerChild3Sprite");
	mChild4Sprite = CreateComponent<CSpriteComponent>("PlayerChild4Sprite");

	SetRootSceneComponent(mSprite);

	mSprite->AddChild(mChildLeftSprite);
	mSprite->AddChild(mChildRightSprite);
	mSprite->AddChild(mMuzzle);
	mSprite->AddChild(mChildRoot);

	mChildLeftSprite->AddChild(mChildLeftMuzzle);
	mChildRightSprite->AddChild(mChildRightMuzzle);

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

	SetRootSceneComponent(mSprite);

	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetWorldPos(200.f, 100.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

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

	return true;
}

void CPlayer2D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	if (GetAsyncKeyState('D') & 0x8000)
	{
		mSprite->AddRelativeRotZ(-180.f * deltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		mSprite->AddRelativeRotZ(180.f * deltaTime);
	}


	if (GetAsyncKeyState('W') & 0x8000)
	{
		mSprite->AddRelativePos(mSprite->GetWorldAxis(AXIS_Y) * 300.f * deltaTime);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		mSprite->AddRelativePos(mSprite->GetWorldAxis(AXIS_Y) * 300.f * -deltaTime);
	}

	static bool bFire = false;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		bFire = true;
	}

	else if (bFire)
	{
		bFire = false;

		CBullet* bullet = mScene->CreateGameObject<CBullet>("Bullet");

		bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(eAXIS::AXIS_Y) * 75.f);
		bullet->SetWorldRot(GetWorldRot());
	}

	mChildRoot->AddWorldRotZ(180.f * deltaTime);
}

void CPlayer2D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CGameObject* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}
