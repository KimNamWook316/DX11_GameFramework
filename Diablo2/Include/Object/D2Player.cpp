#include "D2Player.h"
#include "Input.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "D2PlayerTownIdle.h"

CD2Player::CD2Player() :
	CD2Object()
{
	SetTypeID<CD2Player>();
}

CD2Player::CD2Player(const CD2Player& obj)	:
	CD2Object(obj)
{
}

CD2Player::~CD2Player()
{
}

bool CD2Player::Init()
{
	CGameObject::Init();

	// Sprite
	mSprite = CreateComponent<CSpriteComponent>("Sprite");
	SetRootSceneComponent(mSprite);
	mSprite->SetPivot(0.5f, 0.f, 0.f);

	// Animation
	mSprite->LoadAnimationInstance("Player/Player.anim");
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
	CInput::GetInst()->SetKeyCallBack("LButtonDown", eKeyState::KeyState_Down, this, &CD2Player::OnLButtonClicked);
	CInput::GetInst()->SetKeyCallBack("RButtonDown", eKeyState::KeyState_Down, this, &CD2Player::OnRButtonClicked);

	// Adjust Sprite Size & Collider Size
	AnimationFrameData data = mSprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);
	float width = data.Size.x;
	float height = data.Size.y;
	mSprite->SetWorldScale(width, height, 1.f);

	mBody->SetOffset(0.f, height / 2.f, 0.f);
	mBody->SetExtent(width / 2.f, height / 2.f);

	// TODO : 엑셀 로딩
	// CharInfo
	mCharInfo.MaxMoveSpeed = 300.f;

	// State 설정
	CD2PlayerTownIdle* state = new CD2PlayerTownIdle;
	mStateStack.push((CD2State*)state);

	return true;
}

void CD2Player::Start()
{
	CD2Object::Start();
}

void CD2Player::Update(float deltaTime)
{
	CD2Object::Update(deltaTime);
}

void CD2Player::PostUpdate(float deltaTime)
{
	CD2Object::PostUpdate(deltaTime);
}

CD2Player* CD2Player::Clone()
{
	return new CD2Player(*this);
}

void CD2Player::OnLButtonClicked(float deltaTime)
{
	CD2State* state = mStateStack.top();
	state->PushInput(0, (int)eInputState::MouseLButton);
}

void CD2Player::OnRButtonClicked(float deltaTime)
{
	CD2State* state = mStateStack.top();
	state->PushInput(0, (int)eInputState::MouseRButton);
}

void CD2Player::SetSpriteDir(const Vector2& dir)
{
	// South 축과의 각을 구한다.
	float dirAngle = Vector2(0.f, -1.f).Angle(dir);

	// dir.x > 0 == 오른쪽을 보는 경우
	if (dirAngle <= 11.25f)
	{
		mCharInfo.SpriteDir = eSpriteDir::S;
	}
	else if (dirAngle <= 33.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::SSE;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::SSW;
		}
	}
	else if (dirAngle <= 56.25f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::ES;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::SW;
		}
	}
	else if (dirAngle <= 78.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::EES;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::WWS;
		}
	}
	else if (dirAngle <= 101.25f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::E;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::W;
		}
	}
	else if (dirAngle <= 123.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::EEN;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::WWN;
		}
	}
	else if (dirAngle <= 146.25f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::NE;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::NW;
		}
	}
	else if (dirAngle <= 168.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eSpriteDir::NNE;
		}
		else
		{
			mCharInfo.SpriteDir = eSpriteDir::NNW;
		}
	}
	else if (dirAngle <= 191.25f)
	{
		mCharInfo.SpriteDir = eSpriteDir::N;
	}
	else
	{
		assert(false);
	}
}

void CD2Player::AdjustSpriteSize()
{
	AnimationFrameData data = mSprite->GetAnimationInstance()->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);
	float width = data.Size.x;
	float height = data.Size.y;
	mSprite->SetWorldScale(width, height, 1.f);
}

