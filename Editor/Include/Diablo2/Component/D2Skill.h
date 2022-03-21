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
	class CGameObject* DoSkill(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, class CGameObject* targetObj = nullptr);
	void LoadChildSkill(std::vector<std::vector<std::string>>& vecChildData);

public:
	void LevelUp();
	void LevelUpAll(); // DEBUG

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

	eD2ElementType GetElementType();

	CD2Skill* GetSkill(const std::string& name);

	float GetMp() const
	{
		return mMp;
	}

	float GetRange() const
	{
		return mRange;
	}

private:
	class CD2PlayerSkillComponent* mOwner;
	eD2SkillTreeNo mTreeNo;
	std::string mName;
	eD2AttackType meAttackType;
	float mMp;
	float mRange;
	int mLevel;
	int mMaxLevel;
	CD2Skill* mParentSkill;
	std::vector<CD2Skill*> mVecChildSkill;
	int mPreSkillLevel;
	bool mbLevelUpAvailable;
	bool mbActive;
};

