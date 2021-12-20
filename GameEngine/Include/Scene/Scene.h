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
	void Start();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);

public:
	void Save(const char* fileName, const std::string& pathName = SCENE_PATH);
	bool SaveFullPath(const char* fullPath);
	void Load(const char* fileName, const std::string& pathName = SCENE_PATH);
	bool LoadFullPath(const char* fullPath);

public:
	CGameObject* FindObject(const std::string& name)
	{
		auto iter = mObjList.begin();
		auto iterEnd = mObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == name)
			{
				return (*iter);
			}
		}
		return nullptr;
	}

public:
	CSceneResource* GetResource() const
	{
		return mResource;
	}

	CGameObject* GetPlayerObj() const
	{
		return mMode->GetPlayerObj();
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
		obj->mScene = this;

		if (!obj->Init())
		{
			SAFE_RELEASE(obj);
			return nullptr;
		}

		mObjList.push_back(obj);

		if (mbIsStart)
		{
			obj->Start();
		}

		return obj;
	}

private:
	CSharedPtr<CSceneMode> mMode;
	CSceneResource* mResource;

	std::list<CSharedPtr<CGameObject>> mObjList;
	bool mbIsStart;
};

