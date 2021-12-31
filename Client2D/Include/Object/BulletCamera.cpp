#include "BulletCamera.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CBulletCamera::CBulletCamera()
{
	SetTypeID<CBulletCamera>();
}

CBulletCamera::CBulletCamera(const CBulletCamera& bullet)	:
	CGameObject(bullet)
{
	mSprite = (CSpriteComponent*)FindComponent("BulletSprite");
	mBody = (CColliderBox2D*)FindComponent("BulletBody");
	mCamera = (CCameraComponent*)FindComponent("BulletCamera");
}

CBulletCamera::~CBulletCamera()
{
}

bool CBulletCamera::Init()
{
	CGameObject::Init();

	mSprite = CreateComponent<CSpriteComponent>("BulletSprite");
	mBody = CreateComponent<CColliderBox2D>("BulletBody");
	mCamera = CreateComponent<CCameraComponent>("BulletCamera");

	SetRootSceneComponent(mSprite);
	mSprite->AddChild(mBody);
	mSprite->AddChild(mCamera);

	mCamera->OnViewportCenter();

	mBody->SetCollisionProfile("PlayerAttack");
	mBody->AddCollisionCallBack(eCollisionState::Enter, this, &CBulletCamera::OnCollisionCallBack);

	mSprite->SetWorldScale(50.f, 50.f, 1.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);
	return true;
}

void CBulletCamera::Start()
{
	CGameObject::Start();

	mScene->GetCameraManager()->KeepCamera();
	mScene->GetCameraManager()->SetCurrentCamera(mCamera);
}

void CBulletCamera::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	AddRelativePos(GetWorldAxis(AXIS_Y) * deltaTime * 500.f);
}

void CBulletCamera::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBulletCamera* CBulletCamera::Clone()
{
	return new CBulletCamera(*this);
}

void CBulletCamera::OnCollisionCallBack(const CollisionResult& result)
{
	mScene->GetCameraManager()->ReturnCamera();
	Destroy();
}
