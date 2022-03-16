#pragma once

#include "Component/ObjectComponent.h"

class CD2PlayerCollider :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CD2PlayerCollider();
	CD2PlayerCollider(const CD2PlayerCollider& com);
	virtual ~CD2PlayerCollider();
	
public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CD2PlayerCollider* Clone() override;

public:
	void OnCollideEnter(const CollisionResult& result);
	void OnCollideExit(const CollisionResult& result);

protected:
	CSharedPtr<class CColliderBox2D> mCollider;
	CSharedPtr<class CD2StateComponent> mState;
};

