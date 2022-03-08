#include "D2Object.h"
#include "D2State.h"

CD2Object::CD2Object()	:
	mCharInfo{}
{
	SetTypeID<CD2Object>();
}

CD2Object::CD2Object(const CD2Object& obj)	:
	CGameObject(obj)
{
	mCharInfo = obj.mCharInfo;
}

CD2Object::~CD2Object()
{
}

bool CD2Object::Init()
{
	CGameObject::Init();

	// Sprite
	mSprite = CreateComponent<CSpriteComponent>("Sprite");
	SetRootSceneComponent(mSprite);
	mSprite->SetPivot(0.5f, 0.f, 0.f);

	return true;
}

void CD2Object::Start()
{
	CGameObject::Start();
}

void CD2Object::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	CD2State* curState = mStateStack.top();
	CD2State* newState = curState->GetNextState();
	
	if (newState)
	{
		if (newState->mbIsLoop)
		{
			curState->OnExitState(deltaTime);
			mStateStack.pop();
			SAFE_DELETE(curState);
			mStateStack.push(newState);
		}
		else
		{
			curState->Clear();
			mStateStack.push(newState);
		}
		newState->mOwnerObject = this;
		newState->OnEnterState(deltaTime);
	}
	else
	{
		if (curState->mbIsEnd)
		{
			curState->OnExitState(deltaTime);
			mStateStack.pop();
			SAFE_DELETE(curState);
			curState = mStateStack.top();
			curState->OnEnterState(deltaTime);
		}
	}
	curState->Update(deltaTime);
}

void CD2Object::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CD2Object* CD2Object::Clone()
{
	return new CD2Object(*this);
}

void CD2Object::Save(FILE* fp)
{
	CGameObject::Save(fp);
	fwrite(&mCharInfo, sizeof(CharacterInfo), 1, fp);
}

void CD2Object::Load(FILE* fp)
{
	CGameObject::Load(fp);
	fread(&mCharInfo, sizeof(CharacterInfo), 1, fp);
}

void CD2Object::SetDirection(const Vector2& dir)
{
	mCharInfo.Dir = dir;
	mCharInfo.Dir.Normalize();

	// Sprite Direction 구한다
	SetSpriteDir(mCharInfo.Dir);
}

void CD2Object::SetSpriteDir(const Vector2& dir)
{
	// South 축과의 각을 구한다.
	float dirAngle = Vector2(0.f, -1.f).Angle(dir);

	if (dirAngle <= 22.5f)
	{
		mCharInfo.SpriteDir = eD2SpriteDir::S;
	}
	else if (dirAngle <= 67.5f)
	{
		if (dir.x < 0)
		{
			mCharInfo.SpriteDir = eD2SpriteDir::SW;
		}
		else
		{
			mCharInfo.SpriteDir = eD2SpriteDir::ES;
		}
	}
	else if (dirAngle <= 112.5f)
	{
		if (dir.x < 0)
		{
			mCharInfo.SpriteDir = eD2SpriteDir::W;
		}
		else
		{
			mCharInfo.SpriteDir = eD2SpriteDir::E;
		}
	}
	else if (dirAngle <= 157.5f)
	{
		if (dir.x < 0)
		{
			mCharInfo.SpriteDir = eD2SpriteDir::NW;
		}
		else
		{
			mCharInfo.SpriteDir = eD2SpriteDir::NE;
		}
	}
	else if (dirAngle <= 202.5f)
	{
		mCharInfo.SpriteDir = eD2SpriteDir::N;
	}
	else
	{
		assert(false);
	}
}

void CD2Object::AdjustSpriteSize()
{
}
