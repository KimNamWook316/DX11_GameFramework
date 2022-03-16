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
	virtual bool Move(const Vector3& endPos);
	void SetMoveSpeed(const float speed)
	{
		mMoveSpeed = speed;
	}

public:
	bool IsPathExist()
	{
		return !mPathList.empty();
	}

	const Vector3& GetPathListFront() const
	{
		return mPathList.front();
	}

protected:
	virtual void onFindPathResult(const std::list<Vector3>& pathList);

public:
	template<typename T>
	void SetEndCallBack(T* obj, void(T::* func)())
	{
		mEndCallBack = std::bind(func, obj);
	}

	template<typename T>
	void SetFrameCallBack(T* obj, void(T::* func)())
	{
		mFrameCallBack = std::bind(func, obj);
	}

	void DeleteEndCallBack()
	{
		mEndCallBack = nullptr;
	}

	void DeleteFrameCallBack()
	{
		mFrameCallBack = nullptr;
	}

protected:
    CSharedPtr<class CSceneComponent> mUpdateComponent;
    std::list<Vector3> mPathList;
    float mMoveSpeed;
	std::function<void()> mEndCallBack;
	std::function<void()> mFrameCallBack;
	int mPrevEndIdx;
};

