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
	bool LevelUp(const std::string& name);
	void LevelUpAll(); // DEBUG

	void SetLevelUpAvailable(bool bAvailable);

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

    void GetSkillStateInfo(std::map<std::string, D2SkillStateInfo>& outInfo);

	float GetMp() const
	{
		return mMp;
	}

	float GetRange() const
	{
		return mRange;
	}

public:
	template <typename T>
	void SetStateChangeCallBack(T* obj, void(T::* func)(D2SkillStateInfo))
	{
		std::function<void(D2SkillStateInfo)> callBack = std::bind(func, obj, std::placeholders::_1);
		mStateChangeCallBackList.push_back(callBack);
	}

	template <typename T>
	void SetStateChangeCallBackAll(T* obj, void(T::* func)(D2SkillStateInfo))
	{
		std::function<void(D2SkillStateInfo)> callBack = std::bind(func, obj, std::placeholders::_1);
		mStateChangeCallBackList.push_back(callBack);

		size_t size = mVecChildSkill.size();
		for (size_t i = 0; i < size; ++i)
		{
			mVecChildSkill[i]->SetStateChangeCallBackAll(obj, func);
		}
	}

private:
	void callStateChangeCallBack(bool bFirstLevelUp);

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
	std::list<std::function<void(D2SkillStateInfo)>> mStateChangeCallBackList;
};

