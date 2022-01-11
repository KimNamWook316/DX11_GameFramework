#include "CameraManager.h"

CCameraManager::CCameraManager() :
	mScene(nullptr)
{
}

CCameraManager::~CCameraManager()
{
}

bool CCameraManager::Init()
{
	mCurrentCamera = new CCameraComponent;
	mCurrentCamera->Init();
	mCurrentCamera->SetName("DefaultCamera");
	mCurrentCamera->mScene = mScene;
	mCurrentCamera->SetCameraType(eCameraType::Camera2D);

	mUICamera = new CCameraComponent;
	mUICamera->Init();
	mUICamera->SetName("UICamera");
	mUICamera->mScene = mScene;
	mUICamera->SetCameraType(eCameraType::UI);

	return true;
}

void CCameraManager::Start()
{
	if (mCurrentCamera->GetName() == "DefaultCamera")
	{
		mCurrentCamera->Start();
	}
	
	mUICamera->Start();
}

void CCameraManager::Update(float deltaTime)
{
	if ("DefualtCamera" == mCurrentCamera->GetName())
	{
		mCurrentCamera->Update(deltaTime);
	}
}

void CCameraManager::PostUpate(float deltaTime)
{
	if ("DefualtCamera" == mCurrentCamera->GetName())
	{
		mCurrentCamera->PostUpdate(deltaTime);
	}
}
