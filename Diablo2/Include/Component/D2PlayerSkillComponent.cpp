#include "D2PlayerSkillComponent.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"
#include "Resource/ResourceManager.h"
#include "Scene/Scene.h"
#include "../D2Util.h"
#include "Input.h"

CD2PlayerSkillComponent::CD2PlayerSkillComponent()	:
	mSkillPoint(0),
	mMeleeAttackIdx(0),
	mRSkillIdx(0),
	mbInit(false),
	mArrSkillTree{},
	mMeleeAtack(nullptr)
{
	SetTypeID<CD2PlayerSkillComponent>();
}

CD2PlayerSkillComponent::CD2PlayerSkillComponent(const CD2PlayerSkillComponent& com)	:
	CObjectComponent(com)
{
	mMeleeAttackIdx = com.mMeleeAttackIdx;
	mRSkillIdx = com.mRSkillIdx;

	// TODO : 복사생성 될 일 있으면 구현
	for (int i = 0; i < (int)eD2SkillTreeNo::Max; ++i)
	{
	}
}

CD2PlayerSkillComponent::~CD2PlayerSkillComponent()
{
	SAFE_DELETE(mMeleeAtack);

	for (int i = 0; i < (int)eD2SkillTreeNo::Max; ++i)
	{
		SAFE_DELETE(mArrSkillTree[i]);
	}
}

bool CD2PlayerSkillComponent::Init()
{
	loadSkillList();
	mbInit = true;
	return true;
}

void CD2PlayerSkillComponent::Start()
{
	CObjectComponent::Start();

	if (!mbInit)
	{
		loadSkillList();
	}

	// 레벨업시 스킬포인트 얻는다.
	mObject->FindObjectComponentFromType<CD2CharacterInfoComponent>()->SetEventCallBack("SkillPointUp", this, &CD2PlayerSkillComponent::GainSkillPoint, eD2CharInfoEventType::LevelUp);

	CInput::GetInst()->SetKeyCallBack("SkillLevelUp", eKeyState::KeyState_Down, this, &CD2PlayerSkillComponent::LevelUpAll);
	CInput::GetInst()->SetKeyCallBack("NextSkill", eKeyState::KeyState_Down, this, &CD2PlayerSkillComponent::SetNextRSkill);
}

void CD2PlayerSkillComponent::Update(float deltaTime)
{
}

void CD2PlayerSkillComponent::PostUpdate(float deltaTime)
{
}

void CD2PlayerSkillComponent::PrevRender()
{
}

void CD2PlayerSkillComponent::Render()
{
}

void CD2PlayerSkillComponent::PostRender()
{
}

CD2PlayerSkillComponent* CD2PlayerSkillComponent::Clone()
{
	return new CD2PlayerSkillComponent(*this);
}

void CD2PlayerSkillComponent::LevelUpAll(float deltaTime)
{
	for (int i = 0; i < (int)eD2SkillTreeNo::Max; ++i)
	{
		mArrSkillTree[i]->LevelUpAll();
	}
}

bool CD2PlayerSkillComponent::LevelUp(eD2SkillTreeNo treeNo, const std::string& skillName)
{
	if (mSkillPoint <= 0)
	{
		return false;
	}

	CD2Skill* rootSkill = mArrSkillTree[(int)treeNo];

	if (!rootSkill)
	{
		return false;
	}

	return rootSkill->LevelUp(skillName);
}

int CD2PlayerSkillComponent::GetRSkillType()
{
	if (isValidIdx(mRSkillIdx))
	{
		return (int)mVecAvailableSkill[mRSkillIdx]->GetAttackType();
	}
	return -1;
}

int CD2PlayerSkillComponent::GetRSkillElementType()
{
	if (isValidIdx(mRSkillIdx))
	{
		return (int)mVecAvailableSkill[mRSkillIdx]->GetElementType();
	}
	return -1;
}

void CD2PlayerSkillComponent::SetNextRSkill(float deltaTime)
{
	++mRSkillIdx;

	if (mRSkillIdx >= mVecAvailableSkill.size())
	{
		mRSkillIdx = 0;
	}
}

void CD2PlayerSkillComponent::SetRSkill(const int idx)
{
	if (isValidIdx(idx))
	{
		mRSkillIdx = idx;
	}
}

void CD2PlayerSkillComponent::SetRSkill(const std::string& name)
{
	int idx = findSkillIdx(name);

	if (-1 == idx)
	{
		return;
	}

	mRSkillIdx = idx;
}

void CD2PlayerSkillComponent::GetSkillStateInfo(std::map<std::string ,D2SkillStateInfo>& outInfo)
{
	for (int i = 0; i < (int)eD2SkillTreeNo::Max; ++i)
	{
		mArrSkillTree[i]->GetSkillStateInfo(outInfo);
	}
}

CGameObject* CD2PlayerSkillComponent::DoMeleeAttack(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, CGameObject* targetObj)
{
	return mMeleeAtack->DoSkill(startPos, targetPos, dir, targetObj);
}

CGameObject* CD2PlayerSkillComponent::DoRSkill(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, CGameObject* targetObj)
{
	if (isValidIdx(mRSkillIdx))
	{ 
		return mVecAvailableSkill[mRSkillIdx]->DoSkill(startPos, targetPos, dir, targetObj);
	}
	return nullptr;
}

bool CD2PlayerSkillComponent::isValidIdx(const int idx)
{
	if (idx < 0 || idx >= mVecAvailableSkill.size())
	{
		return false;
	}
	return true;
}

bool CD2PlayerSkillComponent::loadSkillList()
{
	std::string csvName;
	CResourceManager::GetInst()->LoadCSV(csvName,"PlayerSkillList.csv");

	CExcelData* data = CResourceManager::GetInst()->FindCSV(csvName);

	if (!data)
	{
		return false;
	}

	std::unordered_map<std::string, std::vector<std::string>*> table = data->GetTable();

	auto iter = table.begin();
	auto iterEnd = table.end();

	std::vector<std::vector<std::vector<std::string>>> vecChildData;
	vecChildData.resize((int)eD2SkillTreeNo::Max);

	for (; iter != iterEnd; ++iter)
	{
		eD2SkillTreeNo skillTreeNo = CD2Util::StringToSkilltreeNo((*iter->second)[2]);

		// 기본 공격
		if ((*iter->second)[(int)eCSVLabelSkill::Name] == "MeleeAttack")
		{
			CD2Skill* normalAttack = new CD2Skill;
			normalAttack->mOwner = this;
			normalAttack->mName = (*iter->second)[(int)eCSVLabelSkill::Name];
			normalAttack->meAttackType = CD2Util::StringToAttackType((*iter->second)[(int)eCSVLabelSkill::AttackType]);
			normalAttack->mTreeNo = skillTreeNo;
			normalAttack->mLevel = 1;
			normalAttack->mMaxLevel = std::stoi((*iter->second)[(int)eCSVLabelSkill::MaxLevel]);
			normalAttack->mPreSkillLevel = std::stoi((*iter->second)[(int)eCSVLabelSkill::PreSkillLevel]);
			normalAttack->mMp = std::stof((*iter->second)[(int)eCSVLabelSkill::Mp]);
			normalAttack->mRange = std::stof((*iter->second)[(int)eCSVLabelSkill::Range]);
			mMeleeAtack = normalAttack;
			continue;
		}

		// Default 스킬인 경우 스킬트리 최상위 스킬임
		if ((*iter->second)[(int)eCSVLabelSkill::IsDefault] == "TRUE")
		{
			mArrSkillTree[(int)(skillTreeNo)] = new CD2Skill;
			mArrSkillTree[(int)(skillTreeNo)]->mOwner = this;
			mArrSkillTree[(int)(skillTreeNo)]->mName = (*iter->second)[(int)eCSVLabelSkill::Name];
			mArrSkillTree[(int)(skillTreeNo)]->meAttackType = CD2Util::StringToAttackType((*iter->second)[(int)eCSVLabelSkill::AttackType]);
			mArrSkillTree[(int)(skillTreeNo)]->mTreeNo = skillTreeNo;
			mArrSkillTree[(int)(skillTreeNo)]->mLevel = 1;
			mArrSkillTree[(int)(skillTreeNo)]->mMaxLevel = std::stoi((*iter->second)[(int)eCSVLabelSkill::MaxLevel]);
			mArrSkillTree[(int)(skillTreeNo)]->mPreSkillLevel = std::stoi((*iter->second)[(int)eCSVLabelSkill::PreSkillLevel]);
			mArrSkillTree[(int)(skillTreeNo)]->mMp = std::stof((*iter->second)[(int)eCSVLabelSkill::Mp]);
			mArrSkillTree[(int)(skillTreeNo)]->mRange = std::stof((*iter->second)[(int)eCSVLabelSkill::Range]);
			mArrSkillTree[(int)(skillTreeNo)]->mbLevelUpAvailable = true;

			// Active 스킬에 추가
			AddActiveSkill(mArrSkillTree[(int)(skillTreeNo)]);
		}
		// Default Skill이 아닌 경우, 선행 스킬이 있는 경우이므로, 타입에 따라 생성 데이터를 분류한다.
		else
		{
			vecChildData[(int)skillTreeNo].push_back(*iter->second);
		}

	}

	// 최상위 스킬들에서 자식 스킬들 생성
	for (int i = 0; i < (int)eD2SkillTreeNo::Max; ++i)
	{
		mArrSkillTree[i]->LoadChildSkill(vecChildData[i]);
	}

	CResourceManager::GetInst()->DeleteCSV(csvName);
	return true;
}

CD2Skill* CD2PlayerSkillComponent::findSkill(const std::string& name)
{
	size_t size = mVecAvailableSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (mVecAvailableSkill[i]->GetName() == name)
		{
			return mVecAvailableSkill[i];
		}
	}
	return nullptr;
}

int CD2PlayerSkillComponent::findSkillIdx(const std::string& name)
{
	size_t size = mVecAvailableSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (mVecAvailableSkill[i]->GetName() == name)
		{
			return (int)i;
		}
	}
	return -1;
}
