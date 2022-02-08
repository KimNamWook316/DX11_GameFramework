#include "Bullet.h"
#include "Component/SpriteComponent.h"

CBullet::CBullet()	:
	mDistance(600.f)
{
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet& bullet)	:
	CGameObject(bullet)
{
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("BulletSprite");
	mBody = CreateComponent<CColliderCircle>("Body");
	
	SetRootSceneComponent(mSprite);
	mSprite->AddChild(mBody);

	mSprite->SetRelativeScale(50.f, 50.f, 1.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	mBody->AddCollisionCallBack(eCollisionState::Enter, this, &CBullet::OnCollisionEnter);
	mBody->AddCollisionCallBack(eCollisionState::Exit, this, &CBullet::OnCollisionExit);

	return true;
}

void CBullet::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	float dist = 500.f * deltaTime;

	mDistance -= dist;

	if (mDistance < 0.f)
	{
		Destroy();
	}

	AddRelativePos(GetWorldAxis(AXIS_Y) * dist);
}

void CBullet::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::SetCollisionProfile(const std::string& name)
{
	mBody->SetCollisionProfile(name);
}

void CBullet::OnCollisionEnter(const CollisionResult& result)
{
	Destroy();
}

void CBullet::OnCollisionExit(const CollisionResult& result)
{
}
