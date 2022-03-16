#pragma once

#include "Component/ObjectComponent.h"

class CD2EnemyCollider :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CD2EnemyCollider();
	CD2EnemyCollider(const CD2EnemyCollider& com);
	virtual ~CD2EnemyCollider();
	
public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CD2EnemyCollider* Clone() override;

public:
	void OnCollideEnter(const CollisionResult& result);
	void OnCollideExit(const CollisionResult& result);

private:
	void adjustPosition(const CollisionResult& result);

protected:
	CSharedPtr<class CColliderBox2D> mCollider;
	CSharedPtr<class CD2StateComponent> mState;
};

