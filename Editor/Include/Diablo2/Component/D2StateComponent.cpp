#include "D2StateComponent.h"
#include "GameObject/GameObject.h"

CD2StateComponent::CD2StateComponent()
{
	SetTypeID<CD2StateComponent>();
}

CD2StateComponent::CD2StateComponent(const CD2StateComponent& com)	:
	CStateComponent(com)
{
}

CD2StateComponent::~CD2StateComponent()
{
}

void CD2StateComponent::Start()
{
	CStateComponent::Start();

	mCharInfo = mObject->FindObjectComponentFromType<CD2CharacterInfoComponent>();
	mSkill = mObject->FindObjectComponentFromType<CD2PlayerSkillComponent>();
}
