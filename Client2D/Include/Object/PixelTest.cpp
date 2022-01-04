#include "PixelTest.h"
#include "Component/ColliderPixel.h"

CPixelTest::CPixelTest()
{
	SetTypeID<CPixelTest>();
}

CPixelTest::CPixelTest(const CPixelTest& obj)	:
	CGameObject(obj)
{
	mBody = (CColliderPixel*)FindComponent("Body");
}

CPixelTest::~CPixelTest()
{
}

bool CPixelTest::Init()
{
	mBody = CreateComponent<CColliderPixel>("Body");
	SetRootSceneComponent(mBody);
	mBody->SetWorldPos(400.f, 500.f, 0.f);
	mBody->SetInfo(TEXT("PixelCollision.png"));
	mBody->SetPixelCollisionType(ePixelCollisionType::ColorIgnore);
	mBody->SetPixelColor(255, 0, 255);
	return true;
}

CPixelTest* CPixelTest::Clone()
{
	return new CPixelTest(*this);
}
