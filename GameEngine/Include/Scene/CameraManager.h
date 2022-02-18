#pragma once

#include "../GameInfo.h"
#include "../Component/CameraComponent.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

public:
	bool Init();
	void Start();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);

public:
	CCameraComponent* GetCurrentCamera() const
	{
		return mCurrentCamera;
	}

	CCameraComponent* GetUICamera() const
	{
		return mUICamera;
	}

public:
	void SetCurrentCamera(CCameraComponent* camera)
	{
		mCurrentCamera = camera;
	}

	void KeepCamera()
	{
		mKeepCamera = mCurrentCamera;
	}

	void ReturnCamera()
	{
		mCurrentCamera = mKeepCamera;
		mKeepCamera = nullptr;
	}
	
private:
	class CScene* mScene;
	CSharedPtr<CCameraComponent> mCurrentCamera;
	CSharedPtr<CCameraComponent> mKeepCamera;
	CSharedPtr<CCameraComponent> mUICamera;
};

