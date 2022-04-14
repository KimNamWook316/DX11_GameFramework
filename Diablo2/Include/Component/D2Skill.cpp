#include "D2Skill.h"
#include "GameObject/GameObject.h"
#include "D2DataManager.h"
#include "D2PlayerSkillComponent.h"
#include "D2SkillObject.h"
#include "D2ObjectPool.h"
#include "../D2Util.h"
#include "Scene/Scene.h"

CD2Skill::CD2Skill()	:
	mOwner(nullptr),
	meAttackType(eD2AttackType::Melee),
	mLevel(0),
	mMaxLevel(0),
	mParentSkill(nullptr),
	mPreSkillLevel(0),
	mbLevelUpAvailable(false),
	mbActive(false),
	mTreeNo(eD2SkillTreeNo::Max)
{
}

CD2Skill::CD2Skill(const CD2Skill& skill)
{
	*this = skill;

	mOwner = nullptr;
}

CD2Skill::~CD2Skill()
{
	size_t size = mVecChildSkill.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecChildSkill[i]);
	}
	mVecChildSkill.clear();
}

class CGameObject* CD2Skill::DoSkill(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, CGameObject* targetObj)
{
	CGameObject* obj = CD2ObjectPool::GetInst()->CloneSkillObj(mName);

	if (!obj)
	{
		assert(false);
		return nullptr;
	}

	obj->Start();
	obj->Enable(true);
	obj->SetWorldPos(startPos);

	CD2SkillObject* objController = (CD2SkillObject*)obj->FindComponent("Script");
	
	float damage = objController->GetDamage();
	float additional = damage * (mLevel / 100.f);

	objController->SetDamage(damage + additional);
	objController->SetDir(dir);
	objController->SetStartPos(startPos);
	objController->SetTargetPos(targetPos);
	objController->SetSkillOwner(mOwner->GetGameObject());
	objController->SetTargetObject(targetObj);
	
	// Sound
	CSound* sound = mOwner->GetScene()->GetResource()->FindSound(mName);

	if (sound)
	{
		objController->SetSound(sound);
		sound->Play();
	}

	return obj;
}

void CD2Skill::LoadChildSkill(std::vector<std::vector<std::string>>& vecChildData)
{
	CD2Skill* child = nullptr;

	size_t size = vecChildData.size();

	for (size_t i = 0; i < size;)
	{
		// 이 스킬이 부모 스킬이라면, 자식 스킬을 생성하고, 생성한 자식 스킬 데이터는 뺸다.
		if (vecChildData[i][(int)eCSVLabelSkill::ParentSkill] == mName)
		{
			child = new CD2Skill;
			child->mParentSkill = this;
			child->mOwner = mOwner;

			child->mName = vecChildData[i][(int)eCSVLabelSkill::Name];
			child->meAttackType = CD2Util::StringToAttackType(vecChildData[i][(int)eCSVLabelSkill::AttackType]);
			child->mTreeNo = CD2Util::StringToSkilltreeNo(vecChildData[i][(int)eCSVLabelSkill::TreeNo]);
			child->mLevel = 0;
			child->mMaxLevel = std::stoi(vecChildData[i][(int)eCSVLabelSkill::MaxLevel]);
			child->mPreSkillLevel = std::stoi(vecChildData[i][(int)eCSVLabelSkill::PreSkillLevel]);
			child->mMp = std::stof(vecChildData[i][(int)eCSVLabelSkill::Mp]);
			child->mRange = std::stof(vecChildData[i][(int)eCSVLabelSkill::Range]);

			mVecChildSkill.push_back(child);

			vecChildData.erase(vecChildData.begin() + i);
			size = vecChildData.size();
		}
		else
		{
			++i;
		}
	}

	// 자식 생성 데이터가 남아있으면, 아직 생성해야 할 하위 데이터가 있다는 뜻이므로, 자식들을 순회하며 생성한다.
	size = mVecChildSkill.size();
	
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildSkill[i]->LoadChildSkill(vecChildData);
	}
}

bool CD2Skill::LevelUp(const std::string& name)
{
	// 이 스킬이 찾는 스킬이라면
	if (name == mName)
	{
		if (!mbLevelUpAvailable)
		{
			return false;
		}

		if (mLevel == mMaxLevel)
		{
			mbLevelUpAvailable = false;
			return false;
		}

		if (mOwner->GetSkillPoint() <= 0)
		{
			return false;
		}

		++mLevel;
		mOwner->UseSkillPoint();

		// 처음 레벨업되면, 사용 가능한 스킬에 추가
		bool bFirstLevelUp = false;

		if (mLevel == 1)
		{
			bFirstLevelUp = true;
			mOwner->AddActiveSkill(this);
		}

		// 선행 스킬 레벨 충족되면 자식 스킬들 레벨업 가능한 상태로 변경
		size_t size = mVecChildSkill.size();

		for (size_t i = 0; i < size; ++i)
		{
			if (mLevel >= mVecChildSkill[i]->GetPreSkillLevel())
			{
				mVecChildSkill[i]->SetLevelUpAvailable(true);
			}
		}

		callStateChangeCallBack(bFirstLevelUp);

		return true;
	}
	else
	{
		bool found = false;
		size_t size = mVecChildSkill.size();
		for (size_t i = 0; i < size; ++i)
		{
			found = mVecChildSkill[i]->LevelUp(name);

			if (found)
			{
				return true;
			}
		}

		return false;
	}
}

void CD2Skill::LevelUpAll()
{
	if (mLevel == 0)
	{
		mOwner->AddActiveSkill(this);
	}

	mLevel = mMaxLevel;
	mbLevelUpAvailable = false;

	size_t size = mVecChildSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChildSkill[i]->LevelUpAll();
	}
}

void CD2Skill::SetLevelUpAvailable(bool bAvailable)
{
	mbLevelUpAvailable = bAvailable;
	callStateChangeCallBack(false);
}

void CD2Skill::SetOwner(CD2PlayerSkillComponent* owner)
{
	mOwner = owner;
	size_t size = mVecChildSkill.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildSkill[i]->SetOwner(owner);
	}
}

eD2ElementType CD2Skill::GetElementType()
{
	return CD2DataManager::GetInst()->FindSkillInfo(mName)->eElementType;
}

CD2Skill* CD2Skill::GetSkill(const std::string& name)
{
	if (mName == name)
	{
		return this;
	}

	size_t size = mVecChildSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		return mVecChildSkill[i]->GetSkill(name);
	}

	return nullptr;
}

void CD2Skill::GetSkillStateInfo(std::map<std::string, D2SkillStateInfo>& outInfo)
{
	D2SkillStateInfo info;
	info.TreeNo = mTreeNo;
	info.Name = mName;
	info.Level = mLevel;
	info.bLevelUpAvailable = mbLevelUpAvailable;
	outInfo.insert(std::make_pair(mName, info));

	size_t size = mVecChildSkill.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildSkill[i]->GetSkillStateInfo(outInfo);
	}
}

void CD2Skill::callStateChangeCallBack(bool bFirstLevelUp)
{
	D2SkillStateInfo info;
	info.bFirstLevelUp = bFirstLevelUp;
	info.Name = mName;
	info.Level = mLevel;
	info.TreeNo = mTreeNo;
	info.bLevelUpAvailable = mbLevelUpAvailable;

	auto iter = mStateChangeCallBackList.begin();
	auto iterEnd = mStateChangeCallBackList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(info);
	}
}
