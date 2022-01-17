#pragma once

#include "D2Object.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "../Dia2Info.h"

class CPlayerObject final :
    public CD2Object 
{
	friend class CScene;

protected:
	CPlayerObject();
	CPlayerObject(const CPlayerObject& obj);
	virtual ~CPlayerObject();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual CPlayerObject* Clone();

public:
	// 플레이어는 16방향 스프라이트 사용한다.
	virtual void SetSpriteDir(const Vector2& dir);

public:
	void OnLButtonClicked(float deltaTime);
	void OnRButtonClicked(float deltaTime);

private:
	CSharedPtr<CCameraComponent> mCamera;
	CSharedPtr<CSpriteComponent> mSprite;
	CSharedPtr<CColliderBox2D> mBody;
};

