#pragma once

#include "ObjectComponent.h"

class CStateComponent :
    public CObjectComponent 
{
	friend class CGameObject;

protected:
	CStateComponent();
	CStateComponent(const CStateComponent& com);
	virtual ~CStateComponent();

public:
	virtual bool Init() override;
	virtual void Start();
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CStateComponent* Clone() override;

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
	void Clear();

	const size_t GetInitialStateType() const
	{
		return mInitialStateType;
	}

	bool IsStateEmpty() const
	{
		return mStateStack.empty();
	}

	class CSceneComponent* GetRootComponent() const
	{
		return mRoot;
	}

	class CNavAgentComponent* GetNavAgent() const
	{
		return mNavAgent;
	}

public:
	template <typename T>
	void SetInitialState()
	{
		if (!mStateStack.empty())
		{
			return;
		}

		T* initialState = new T;
		if (!initialState->Init())
		{
			SAFE_DELETE(initialState);
			initialState = nullptr;
			return;
		}

		initialState->SetOwner(this);
		initialState->Start();
		mInitialStateType = typeid(T).hash_code();
		mStateStack.push(initialState);
	}

protected:
	size_t mInitialStateType;
	std::stack<class CState*> mStateStack;
	CSharedPtr<class CSceneComponent> mRoot;
	CSharedPtr<class CNavAgentComponent> mNavAgent;
	bool mbStart;
};

