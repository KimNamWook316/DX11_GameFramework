#pragma once

#include "Component/StateComponent.h"
#include "D2CharacterInfoComponent.h"
#include "D2PlayerSkillComponent.h"

class CD2StateComponent :
    public CStateComponent
{
	friend class CGameObject;

protected:
	CD2StateComponent();
	CD2StateComponent(const CD2StateComponent& com);
	virtual ~CD2StateComponent();

public:
	virtual void Start() override;

public:
	CD2CharacterInfoComponent* GetCharInfo() const
	{
		return mCharInfo;
	}

	CD2PlayerSkillComponent* GetSkill() const
	{
		return mSkill;
	}

protected:
	CSharedPtr<CD2CharacterInfoComponent> mCharInfo;
	CSharedPtr<CD2PlayerSkillComponent> mSkill;
};

