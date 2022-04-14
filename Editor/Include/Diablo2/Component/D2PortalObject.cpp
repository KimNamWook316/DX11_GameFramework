#include "D2PortalObject.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "D2ObjectPool.h"
#include "D2StateComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/D2EndingScene.h"
#include "../UI/D2ClickableObjectWindow.h"
#include "D2ClickableUIComponent.h"
#include "D2UIManager.h"

CD2PortalObject::CD2PortalObject()	:
	mSprite(nullptr)
{
	SetTypeID<CD2PortalObject>();
}

CD2PortalObject::CD2PortalObject(const CD2PortalObject& com)	:
	CD2ClickableObject(com)
{
}

CD2PortalObject::~CD2PortalObject()
{
	mObject->GetScene()->GetResource()->SoundStop("Portal");
}

bool CD2PortalObject::Init()
{
	CD2ClickableObject::Init();

	return true;
}

void CD2PortalObject::Start()
{
	CD2ClickableObject::Start();

	if (!mSprite)
	{
		mSprite = (CSpriteComponent*)mObject->GetRootSceneComponent();
	}
	
	mSprite->SetCurrentAnimation("IzualAppear0");
	mSprite->GetAnimationInstance()->SetEndCallBack("IzualAppear0", this, &CD2PortalObject::onAppearAnimEnd);

	static_cast<CD2ClickableObjectWindow*>(mUI->GetWidgetWindow())->SetText("Portal");

	mObject->GetScene()->GetResource()->SoundPlay("PortalOpen");
}

void CD2PortalObject::Update(float deltaTime)
{
}

void CD2PortalObject::PostUpdate(float deltaTime)
{
}

void CD2PortalObject::PrevRender()
{
}

void CD2PortalObject::Render()
{
}

void CD2PortalObject::PostRender()
{
}

CD2PortalObject* CD2PortalObject::Clone()
{
	return new CD2PortalObject(*this);
}

void CD2PortalObject::OnClick()
{
	CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CD2EndingScene>(false);
}

void CD2PortalObject::onAppearAnimEnd()
{
	mSprite->SetCurrentAnimation("IzualIdle0");
	mObject->GetScene()->GetResource()->SoundPlay("Portal");
}
