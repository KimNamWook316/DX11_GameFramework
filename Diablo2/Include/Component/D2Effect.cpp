#include "D2Effect.h"
#include "D2ObjectPool.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

CD2Effect::CD2Effect()
{
	SetTypeID<CD2Effect>();
}

CD2Effect::CD2Effect(const CD2Effect& com)	:
	CObjectComponent(com)
{
}

CD2Effect::~CD2Effect()
{
}

bool CD2Effect::Init()
{
	return true;
}

void CD2Effect::Start()
{
	CObjectComponent::Start();

	std::string animName = mObject->GetName() + "0";

	static_cast<CSpriteComponent*>(mObject->GetRootSceneComponent())->SetEndCallBack(animName, this, &CD2Effect::OnAnimEnd);
	static_cast<CSpriteComponent*>(mObject->GetRootSceneComponent())->GetAnimationInstance()->Replay();
	static_cast<CSpriteComponent*>(mObject->GetRootSceneComponent())->GetAnimationInstance()->Play();
}

void CD2Effect::Update(float deltaTime)
{
}

void CD2Effect::PostUpdate(float deltaTime)
{
}

void CD2Effect::PrevRender()
{
}

void CD2Effect::Render()
{
}

void CD2Effect::PostRender()
{
}

CD2Effect* CD2Effect::Clone()
{
	return new CD2Effect(*this);
}

void CD2Effect::OnAnimEnd()
{
	CD2ObjectPool::GetInst()->ReturnEffect(mObject->GetName(), mObject);
}
