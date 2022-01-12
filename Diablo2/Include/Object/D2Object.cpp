#include "D2Object.h"

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
	return true;
}

void CD2Object::Start()
{
	CGameObject::Start();
}

void CD2Object::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
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

	// South 축과의 각을 구한다.
	float dirAngle = Vector2(0.f, -1.f).Angle(dir);

	// dir.x > 0 == 오른쪽을 보는 경우
	if (dirAngle <= 11.25f)
	{
		mCharInfo.SpriteDir = eCharDir::S;
	}
	else if (dirAngle <= 33.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::SSE;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::SSW;
		}
	}
	else if (dirAngle <= 56.25f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::ES;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::SW;
		}
	}
	else if (dirAngle <= 78.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::EES;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::WWS;
		}
	}
	else if (dirAngle <= 101.25f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::E;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::W;
		}
	}
	else if (dirAngle <= 123.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::EEN;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::WWN;
		}
	}
	else if (dirAngle <= 146.25f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::NE;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::NW;
		}
	}
	else if (dirAngle <= 168.75f)
	{
		if (mCharInfo.Dir.x > 0)
		{
			mCharInfo.SpriteDir = eCharDir::NNE;
		}
		else
		{
			mCharInfo.SpriteDir = eCharDir::NNW;
		}
	}
	else if (dirAngle <= 191.25f)
	{
		mCharInfo.SpriteDir = eCharDir::N;
	}
}
