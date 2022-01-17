#include "PlayerObject.h"
#include "Input.h"

CPlayerObject::CPlayerObject() :
	CD2Object()
{
	SetTypeID<CPlayerObject>();
}

CPlayerObject::CPlayerObject(const CPlayerObject& obj)	:
	CD2Object(obj)
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
	CInput::GetInst()->CreateKey("LButtonDown", VK_LBUTTON);
	CInput::GetInst()->CreateKey("RButtonDown", VK_RBUTTON);
	CInput::GetInst()->SetKeyCallBack("LButtonDown", eKeyState::KeyState_Down, this, &CPlayerObject::OnLButtonClicked);
	CInput::GetInst()->SetKeyCallBack("RButtonDown", eKeyState::KeyState_Down, this, &CPlayerObject::OnRButtonClicked);

	float width = mSprite->GetMaterial()->GetTexture()->GetWidth();
	float height = mSprite->GetMaterial()->GetTexture()->GetHeight();

	mSprite->SetWorldScale(width / 15.f, height / 15.f, 1.f);
	mBody->SetOffset(0.f, height / 30.f, 0.f);
	mBody->SetExtent(width / 30.f, height / 30.f);

	// CharInfo
	mCharInfo.MaxMoveSpeed = 300.f;

	return true;
}

void CPlayerObject::Start()
{
	CD2Object::Start();
}

void CPlayerObject::Update(float deltaTime)
{
	CD2Object::Update(deltaTime);

	AddWorldPos(Vector3(mCharInfo.Dir.x, mCharInfo.Dir.y, 0.f) * deltaTime * mCharInfo.CurMoveSpeed);
}

void CPlayerObject::PostUpdate(float deltaTime)
{
	CD2Object::PostUpdate(deltaTime);
}

CPlayerObject* CPlayerObject::Clone()
{
	return new CPlayerObject(*this);
}

void CPlayerObject::SetSpriteDir(const Vector2& dir)
{
	// South ����� ���� ���Ѵ�.
	float dirAngle = Vector2(0.f, -1.f).Angle(dir);

 //	// dir.x > 0 == �������� ���� ���
 //	if (dirAngle <= 11.25f)
 //	{
 //		mCharInfo.SpriteDir = ePlayerSpriteDir::S;
 //	}
 //	else if (dirAngle <= 33.75f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::SSE;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::SSW;
 //		}
 //	}
 //	else if (dirAngle <= 56.25f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::ES;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::SW;
 //		}
 //	}
 //	else if (dirAngle <= 78.75f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::EES;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::WWS;
 //		}
 //	}
 //	else if (dirAngle <= 101.25f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::E;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::W;
 //		}
 //	}
 //	else if (dirAngle <= 123.75f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::EEN;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::WWN;
 //		}
 //	}
 //	else if (dirAngle <= 146.25f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::NE;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::NW;
 //		}
 //	}
 //	else if (dirAngle <= 168.75f)
 //	{
 //		if (mCharInfo.Dir.x > 0)
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::NNE;
 //		}
 //		else
 //		{
 //			mCharInfo.SpriteDir = ePlayerSpriteDir::NNW;
 //		}
 //	}
 //	else if (dirAngle <= 191.25f)
 //	{
 //		mCharInfo.SpriteDir = ePlayerSpriteDir::N;
 //	}
}

void CPlayerObject::OnLButtonClicked(float deltaTime)
{
}

void CPlayerObject::OnRButtonClicked(float deltaTime)
{
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();
	SetDirection(mouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y));
	
	SetMoveSpeed(mCharInfo.MaxMoveSpeed);
	//std::string animName = "TownWalk" + std::to_string(mCharInfo.SpriteDir);
	//mSprite->ChangeAnimation(animName);
}
