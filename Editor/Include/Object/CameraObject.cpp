#include "CameraObject.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CCameraObject::CCameraObject()
{
	SetTypeID<CCameraObject>();
}

CCameraObject::CCameraObject(const CCameraObject& obj)
{
	mCamera = obj.mCamera;
}

CCameraObject::~CCameraObject()
{
}

bool CCameraObject::Init()
{
	mCamera = CreateComponent<CCameraComponent>("Camera");
	SetRootSceneComponent(mCamera);
	mScene->GetCameraManager()->SetCurrentCamera(mCamera);
	SetWorldScale(1.f, 1.f, 1.f);
	SetWorldPos(0.f, 0.f, 0.f);
	return true;
}

void CCameraObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CCameraObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CCameraObject* CCameraObject::Clone()
{
	return new CCameraObject(*this);
}
