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

	DECLARE_SINGLE(CSceneManager)

private:
	CScene* mScene;
	CScene* mNextScene;
};

