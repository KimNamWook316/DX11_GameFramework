#pragma once

#include "Scene.h"

class CSceneManager
{
public:
	bool Init();
	void Start();
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);

public:
	void CreateNextScene(bool bBeChangeNow = true);
	void ChangeNextScene();

public:
	CScene* GetScene() const
	{
		return mScene;
	}

public:
	void CallCreateSceneModeFunction(CScene* scene, size_t type)
	{
		if (mCreateSceneModeCallBack)
		{
			mCreateSceneModeCallBack(scene, type);
		}
	}

	CGameObject* CallCreateObjectFunction(CScene* scene, size_t type)
	{
		if (mCreateObjectCallBack)
		{
			return mCreateObjectCallBack(scene, type);
		}
		return nullptr;
	}

	class CComponent* CallCreateComponentFunction(CGameObject* obj, size_t type)
	{
		if (mCreateComponentCallBack)
		{
			return mCreateComponentCallBack(obj, type);
		}
		return nullptr;
	}

	void CallCreateAnimInstanceFunction(class CSpriteComponent* comp, size_t type)
	{
		if (mCreateAnimInstanceCallBack)
		{
			mCreateAnimInstanceCallBack(comp, type);
		}
	}

	void CallCreateStateFunction(class CStateComponent* comp, size_t type)
	{
		if (mCreateStateCallBack)
		{
			mCreateStateCallBack(comp, type);
		}
	}

public:
	template<typename T>
	bool CreateSceneMode(bool current = true)
	{
		if (current)
		{
			return mScene->CreateSceneMode<T>();
		}
		else
		{
			return mNextScene->CreateSceneMode<T>();
		}
	}

	template <typename T>
	T* CreateSceneModeNotInitilized(bool bCurrent = true)
	{
		if (bCurrent)
		{
			return mScene->CreateSceneModeNotInitilized<T>();
		}
		return mNextScene->CreateSceneModeNotInitilized<T>();
	}

	template<typename T>
	void SetCreateSceneModeCallBack(T* obj, void(T::* func)(CScene*, size_t))
	{
		mCreateSceneModeCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetCreateObjectCallBack(T* obj, CGameObject*(T::* func)(CScene*, size_t))
	{
		mCreateObjectCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetCreateComponentCallBack(T* obj, CComponent* (T::* func)(CGameObject*, size_t))
	{
		mCreateComponentCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetCreateAnimInstanceCallBack(T* obj, void(T::* func)(class CSpriteComponent*, size_t))
	{
		mCreateAnimInstanceCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetCreateStateCallBack(T* obj, void(T::* func)(class CStateComponent*, size_t))
	{
		mCreateStateCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

private:
	bool changeScene();

	DECLARE_SINGLE(CSceneManager)

private:
	CRITICAL_SECTION mCrt;
	CScene* mScene;
	CScene* mNextScene;
	std::function<void(CScene*, size_t)> mCreateSceneModeCallBack;
	std::function<CGameObject*(CScene*, size_t)> mCreateObjectCallBack;
	std::function<class CComponent* (CGameObject*, size_t)> mCreateComponentCallBack;
	std::function<void(class CSpriteComponent*, size_t)> mCreateAnimInstanceCallBack;
	std::function<void(class CStateComponent*, size_t)> mCreateStateCallBack;
};

