#pragma once
#include "../Ref.h"
class CComponent : public CRef
{
	// GameObject���� �������� ��
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

	class CGameObject* GetObject() const
	{
		return mObject;
	}

	Component_Type GetComponentType() const
	{
		return meComponentType;
	}

public:
	void SetScene(class CScene* scene);
	void SetGameObject(class CGameObject* object);

public:
	virtual bool Init();
	virtual void Update(float deltaTime) = 0;
	virtual void PostUpdate(float deltaTime) = 0;
	virtual void PrevRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual CComponent* Clone() = 0;

protected:
	// � ������Ʈ Ÿ������
	Component_Type meComponentType;

	// ��� ��, ��� ���� ������Ʈ �Ҽ�����
	class CScene* mScene;
	class CGameObject* mObject;
	
};
