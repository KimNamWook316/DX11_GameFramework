#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "../GameObject/GameObject.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

public:
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);

public:
	CSceneResource* GetResource() const
	{
		return mResource;
	}

public:
	template <typename T>
	bool CreateSceneMode()
	{
		mMode = new T;
		mMode->mScene = this;

		if (!mMode->Init())
		{
			mMode = nullptr;
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateGameObject(const std::string& name)
	{
		T* obj = new T;

		// CRef -> SetName
		obj->SetName(name);
		obj->mScene(this);

		if (!obj->Init())
		{
			SAFE_RELEASE(obj);
			return false;
		}

		mObjList.push_back(obj);
		return obj;
	}

private:
	CSharedPtr<CSceneMode> mMode;
	CSceneResource* mResource;

	std::list<CSharedPtr<CGameObject>> mObjList;
};

