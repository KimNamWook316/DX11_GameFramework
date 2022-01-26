#pragma once

#include "D2Object.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "../Component/InputStackComponent.h"

class CD2Player final :
    public CD2Object 
{
	friend class CScene;
	friend class CD2PlayerTownIdle;
	friend class CD2PlayerRun;

protected:
	CD2Player();
	CD2Player(const CD2Player& obj);
	virtual ~CD2Player();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual CD2Player* Clone();

public:
	// 플레이어는 16방향 스프라이트 사용한다.
	virtual void SetSpriteDir(const Vector2& dir) override;

public:
	void OnLButtonClicked(float deltaTime);
	void OnRButtonClicked(float deltaTime);

public:
	CInputStackComponent* GetInputStackComponent()
	{
		return mInputStack;
	}

protected:
	virtual void AdjustSpriteSize() override;

private:
	// Components
	CSharedPtr<CCameraComponent> mCamera;
	CSharedPtr<CColliderBox2D> mBody;
	CSharedPtr<CInputStackComponent> mInputStack;
};

