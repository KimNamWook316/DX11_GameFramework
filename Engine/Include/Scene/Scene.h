#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "Viewport.h"
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

	CSceneCollision* GetCollision() const
	{
		return mCollision;
	}

	CCameraManager* GetCameraManager() const
	{
		return mCameraManager;
	}

	CViewport* GetViewport() const
	{
		return mViewport;
	}

	void GetObjNames(std::vector<std::string>& outNames)
	{
		auto iter = mObjList.begin();
		auto iterEnd = mObjList.end();

		for (;iter != iterEnd; ++iter)
		{
			outNames.push_back((*iter)->GetName());
		}
	}

public:
	void SetBeChange(bool bChange)
	{
		mbBeChange = bChange;
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
	T* CreateSceneModeNotInitilized()
	{
		mMode = new T;
		mMode->mScene = this;

		return (T*)*mMode;
	}

	// 파일로부터 로드할 때 -> Init하지 않고, 파일로부터 오브젝트들을 생성한다.
	template <typename T>
	bool LoadSceneMode()
	{
		mMode = new T;
		mMode->mScene = this;
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

	template <typename T>
	T* LoadGameObject()
	{
		T* obj = new T;
		obj->SetScene(this);
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
	CSceneCollision* mCollision;
	CCameraManager* mCameraManager;
	CViewport* mViewport;

	std::list<CSharedPtr<CGameObject>> mObjList;
	bool mbIsStart;
	bool mbBeChange;
};

