#pragma once

#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CGameObject : public CRef
{
	friend class CScene;

protected:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

public:
	class CScene* GetScene() const
	{
		return mScene;
	}

	void SetRootComponent(CSceneComponent* component)
	{
		mRootComponent = component;
	}
	
public:
	void SetScene(class CScene* scene);

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CGameObject* Clone();

public:
	template <typename T>
	T* CreateComponent(const std::string& name)
	{
		T* comp = new T;

		comp->SetName(name);
		comp->SetScene(mScene);
		comp->SetGameObject(this);

		if (!comp->Init())
		{
			SAFE_RELEASE(comp);
			return nullptr;
		}

		if (comp->GetComponentType() == Component_Type::OBJ_COMP)
		{
			mVecObjectComponent.push_back(comp);
		}
		
		// ��������Ʈ�� ���� �����̹Ƿ� ���� ����
		return comp;
	}

protected:
	class CScene* mScene;
	CSharedPtr<CSceneComponent> mRootComponent; // �� ������Ʈ�� ���� ������ ����
	std::vector<CSharedPtr<CObjectComponent>> mVecObjectComponent; // ����, ���� ���� ������, �� ������Ʈ�� ������ �� ����
};

