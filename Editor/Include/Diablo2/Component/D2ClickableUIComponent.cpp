#include "D2ClickableUIComponent.h"
#include "../UI/D2ClickableObjectWindow.h"
#include "GameObject/GameObject.h"
#include "D2UIManager.h"

CD2ClickableUIComponent::CD2ClickableUIComponent()	:
	mbInit(false)
{
	SetTypeID<CD2ClickableUIComponent>();
	SetWidgetSpace(eWidgetComponentSpace::World);
	mbIsRender = false;
	EnableOpacity(true);
}

CD2ClickableUIComponent::CD2ClickableUIComponent(const CD2ClickableUIComponent& com)	:
	CWidgetComponent(com)
{
	mbIsRender = false;
}

CD2ClickableUIComponent::~CD2ClickableUIComponent()
{
}

bool CD2ClickableUIComponent::Init()
{
	return true;
}

void CD2ClickableUIComponent::Start()
{
	if (!mWidgetWindow)
	{
		CreateWidgetWindow<CD2ClickableObjectWindow>("ClickableUI");
		mWidgetWindow->SetSize(150.f, 100.f);
	}
}

void CD2ClickableUIComponent::Update(float deltaTime)
{
	CWidgetComponent::Update(deltaTime);

	if (!mbInit)
	{
		mbInit = true;
		CD2UIManager::GetInst()->AddClickableUI(this);
	}
}

void CD2ClickableUIComponent::PostUpdate(float deltaTime)
{
	Vector3 worldPos = GetWorldPos();
	Vector3 worldScale = mObject->GetRootSceneComponent()->GetWorldScale();
	SetRelativePos(-worldScale.x / 2.f, 40.f , 0.f);

	CWidgetComponent::PostUpdate(deltaTime);
}

CD2ClickableUIComponent* CD2ClickableUIComponent::Clone()
{
	return new CD2ClickableUIComponent(*this);
}

void CD2ClickableUIComponent::Save(FILE* fp)
{
	CWidgetComponent::Save(fp);
}

void CD2ClickableUIComponent::Load(FILE* fp)
{
	CWidgetComponent::Load(fp);
}
