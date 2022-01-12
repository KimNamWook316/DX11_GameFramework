#pragma once

#include "GameObject/GameObject.h"
#include "../Dia2Info.h"

class CD2Object :
    public CGameObject
{
    friend class CScene;

protected:
    CD2Object();
    CD2Object(const CD2Object& obj);
    virtual ~CD2Object();

public:
	virtual bool Init() override;
	virtual void Start() override;;
	virtual void Update(float deltaTime) override;;
	virtual void PostUpdate(float deltaTime) override;
	virtual CD2Object* Clone() override;

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
	void SetDirection(const Vector2& dir);

	void SetDirection(const Vector3& dir)
	{
		SetDirection(Vector2(dir.x, dir.y));
	}

	void SetMaxMoveSpeed(const float& speed)
	{
		mCharInfo.MaxMoveSpeed = speed;
	}

	void SetMoveSpeed(const float& speed)
	{
		mCharInfo.CurMoveSpeed = speed;
		if (mCharInfo.CurMoveSpeed > mCharInfo.MaxMoveSpeed)
		{
			mCharInfo.CurMoveSpeed = mCharInfo.MaxMoveSpeed;
		}
	}

protected:
    CharacterInfo mCharInfo;
};

