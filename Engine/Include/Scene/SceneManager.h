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
	CScene* GetScene() const
	{
		return mScene;
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

	template<typename T>
	void SetCreateSceneModeCallBack(T* obj, void(T::* func)(CScene*, size_t))
	{
		mCreateSceneModeCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void SetCreateObjectCallBack(T* obj, void(T::* func)(CScene*, size_t))
	{
		mCreateObjectCallBack = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}

	DECLARE_SINGLE(CSceneManager)

private:
	CScene* mScene;
	CScene* mNextScene;
	std::function<void(CScene*, size_t)> mCreateSceneModeCallBack;
	std::function<void(CScene*, size_t)> mCreateObjectCallBack;
};

