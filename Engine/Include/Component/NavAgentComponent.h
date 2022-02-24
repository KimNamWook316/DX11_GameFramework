#pragma once

#include "ObjectComponent.h"

class CNavAgentComponent :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CNavAgentComponent();
    CNavAgentComponent(const CNavAgentComponent& com);
    virtual ~CNavAgentComponent();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CNavAgentComponent* Clone() override;

public:
	void SetUpdateComponent(class CSceneComponent* updateComponent);
	bool Move(const Vector3& endPos);
	void SetMoveSpeed(const float speed)
	{
		mMoveSpeed = speed;
	}

private:
	void onFindPathResult(const std::list<Vector3>& pathList);

private:
    CSharedPtr<class CSceneComponent> mUpdateComponent;
    std::list<Vector3> mPathList;
    float mMoveSpeed;
};

