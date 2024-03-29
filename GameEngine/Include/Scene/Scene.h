#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "CameraManager.h"
#include "NavigationManager.h"
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
	bool SaveGameObject(const std::string& objName, const char* fileName, const std::string& pathName = OBJECT_PATH);
	bool SaveGameObjectFullPath(const std::string& objName, const char* fullPath);
	CGameObject* LoadGameObject(std::string& outName, const char* fileName, const std::string& pathName = OBJECT_PATH);
	CGameObject* LoadGameObjectFullPath(std::string& outName, const char* fullPath);

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

	CNavigationManager* GetNavigationManager() const
	{
		return mNavManager;
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

	void SetPlayerObj(CGameObject* obj)
	{
		mMode->SetPlayerObj(obj);
	}
	
	void AddObject(CGameObject* obj)
	{
		mObjList.push_back(obj);
		callCreatCallBack();
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

		callCreatCallBack();
		return obj;
	}

	template <typename T>
	T* LoadGameObjectByType()
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

	// Start는 LoadScene함수에서 수행
	template <typename T>
	T* CreateEmptyObjectByType()
	{
		T* obj = new T;
		obj->SetScene(this);
		mObjList.push_back(obj);
		
		return obj;
	}

	template <typename T>
	void AddCreateCallBack(T* obj, void(T::* func)())
	{
		std::function<void()> callBack = std::bind(func, obj);
		mCreateObjectCallBackList.push_back(callBack);
	}

private:
	void callCreatCallBack();

private:
	CSharedPtr<CSceneMode> mMode;
	CSceneResource* mResource;
	CSceneCollision* mCollision;
	CCameraManager* mCameraManager;
	CNavigationManager* mNavManager;
	CViewport* mViewport;

	std::list<CSharedPtr<CGameObject>> mObjList;
	bool mbIsStart;
	bool mbBeChange;

	std::list<std::function<void()>> mCreateObjectCallBackList;
};

