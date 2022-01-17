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

	// Sprite Direction 구한다
	SetSpriteDir(mCharInfo.Dir);
}

void CD2Object::SetSpriteDir(const Vector2& dir)
{
	// South 축과의 각을 구한다.
	float dirAngle = Vector2(0.f, -1.f).Angle(dir);

	// dir.x > 0 == 오른쪽을 보는 경우
	if (dirAngle <= 22.5f)
	{
		//mCharInfo.SpriteDir = ePlayerSpriteDir::S;
	}
	else if (dirAngle <= 67.5f)
	{
	}

}
