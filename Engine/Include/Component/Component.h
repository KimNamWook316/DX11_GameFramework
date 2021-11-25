#pragma once

#include "../Ref.h"

class CComponent : public CRef
{
	// GameObject에서 생성해줄 것
	friend class CGameObject;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent();

public:
	class CScene* GetScene() const
	{
		return mScene;
	}

	class CGameObject* GetGameObject() const
	{
		return mObject;
	}

	eComponentType GetComponentType() const
	{
		return meComponentType;
	}

public:
	virtual void SetScene(class CScene* scene);
	virtual void SetGameObject(class CGameObject* object);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime) = 0;
	virtual void PostUpdate(float deltaTime) = 0;
	virtual void PrevRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual CComponent* Clone() = 0;

protected:
	// 어떤 컴포넌트 타입인지
	eComponentType meComponentType;

	// 어느 씬, 어느 게임 오브젝트 소속인지
	class CScene* mScene;
	class CGameObject* mObject;
};

