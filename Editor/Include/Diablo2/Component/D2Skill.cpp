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

	static_cast<CD2SkillObject*>(obj->FindComponent("Script"))->SetSkillOwner(mOwner->GetGameObject());
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

	return obj;
}

void CD2Skill::LoadChildSkill(std::vector<std::vector<std::string>>& vecChildData)
{
	CD2Skill* child = nullptr;

	size_t size = vecChildData.size();

	for (size_t i = 0; i < size;)
	{
		// 이 스킬이 부모 스킬이라면, 자식 스킬을 생성하고, 생성한 자식 스킬 데이터는 뺸다.
		if (vecChildData[i][eCSVLabel::ParentSkill] == mName)
		{
			child = new CD2Skill;
			child->mParentSkill = this;
			child->mOwner = mOwner;

			child->mName = vecChildData[i][eCSVLabel::Name];
			child->meAttackType = CD2Util::StringToAttackType(vecChildData[i][eCSVLabel::AttackType]);
			child->mTreeNo = CD2Util::StringToSkilltreeNo(vecChildData[i][eCSVLabel::TreeNo]);
			child->mLevel = 0;
			child->mMaxLevel = std::stoi(vecChildData[i][eCSVLabel::MaxLevel]);
			child->mPreSkillLevel = std::stoi(vecChildData[i][eCSVLabel::PreSkillLevel]);

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

void CD2Skill::LevelUp()
{
	if (mLevel == mMaxLevel)
	{
		mbLevelUpAvailable = false;
		return;
	}

	++mLevel;

	// 처음 레벨업되면, 사용 가능한 스킬에 추가
	if (mLevel == 1)
	{
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

void CD2Skill::SetOwner(CD2PlayerSkillComponent* owner)
{
	mOwner = owner;
	size_t size = mVecChildSkill.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChildSkill[i]->SetOwner(owner);
	}
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