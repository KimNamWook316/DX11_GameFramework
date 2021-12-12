#include "SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"

CSpriteEditObject::CSpriteEditObject()	:
	mSprite(nullptr)
{
}

CSpriteEditObject::CSpriteEditObject(const CSpriteEditObject& obj)	:
	CGameObject(obj)
{
}

CSpriteEditObject::~CSpriteEditObject()
{
}

bool CSpriteEditObject::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("SpriteEdit");
	
	SetRootSceneComponent(mSprite);

	mSprite->SetRelativeScale(100.f, 100.f, 1.f);

	mSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	return true;
}

void CSpriteEditObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CSpriteEditObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CSpriteEditObject* CSpriteEditObject::Clone()
{
	return new CSpriteEditObject(*this);
}
