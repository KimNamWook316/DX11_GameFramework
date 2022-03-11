#pragma once

#include "GameInfo.h"
#include "../D2Info.h"

class CD2Skill
{
	friend class CD2Skill;
	friend class CD2PlayerSkillComponent;

private:
	CD2Skill();
	CD2Skill(const CD2Skill& skill);
	~CD2Skill();

public:
	void Start(const char* objPath);
	class CGameObject* DoSkill(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, class CGameObject* targetObj = nullptr);
	void LoadChildSkill(std::vector<std::vector<std::string>>& vecChildData);

public:
	void LevelUp();

	void SetLevelUpAvailable(bool bAvailable)
	{
		mbLevelUpAvailable = bAvailable;
	}

	void SetOwner(class CD2PlayerSkillComponent* owner);

public:
	const std::string& GetName() const
	{
		return mName;
	}

	int GetPreSkillLevel() const
	{
		return mPreSkillLevel;
	}

	eD2AttackType GetAttackType() const
	{
		return meAttackType;
	}

	CD2Skill* GetSkill(const std::string& name);

private:
	void loadSkillObject(const char* objPath);

private:
	class CD2PlayerSkillComponent* mOwner;
	eD2SkillTreeNo mTreeNo;
	std::string mName;
	eD2AttackType meAttackType;
	class CGameObject* mSkillObject;
	int mLevel;
	int mMaxLevel;
	CD2Skill* mParentSkill;
	std::vector<CD2Skill*> mVecChildSkill;
	int mPreSkillLevel;
	bool mbLevelUpAvailable;
	bool mbActive;
};

