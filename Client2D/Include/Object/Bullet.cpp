#include "Bullet.h"
#include "Component/SpriteComponent.h"

CBullet::CBullet()
{
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
	
	SetRootSceneComponent(mSprite);

	SetRelativeScale(50.f, 50.f, 1.f);
	SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CBullet::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CBullet::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
	
	AddWorldPos(GetWorldAxis(AXIS_Y) * 800.f * deltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}
