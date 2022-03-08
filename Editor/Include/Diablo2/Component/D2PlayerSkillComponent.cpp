#include "D2PlayerSkillComponent.h"
#include "GameObject/GameObject.h"
#include "Resource/ResourceManager.h"
#include "Scene/Scene.h"
#include "../D2Util.h"

CD2PlayerSkillComponent::CD2PlayerSkillComponent()	:
	mLSkillIdx(-1),
	mRSkillIdx(-1)
{
}

CD2PlayerSkillComponent::CD2PlayerSkillComponent(const CD2PlayerSkillComponent& com)	:
	CObjectComponent(com)
{
	mLSkillIdx = com.mLSkillIdx;
	mRSkillIdx = com.mRSkillIdx;
}

CD2PlayerSkillComponent::~CD2PlayerSkillComponent()
{
	size_t size = mVecSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecSkill[i]);
	}
	mVecSkill.clear();
}

bool CD2PlayerSkillComponent::Init()
{
	loadSkillList();
	return true;
}

void CD2PlayerSkillComponent::Start()
{
	if (mVecSkill.empty())
	{
		loadSkillList();
	}
	CObjectComponent::Start();
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

int CD2PlayerSkillComponent::GetLSkillType()
{
	if (isValidIdx(mLSkillIdx))
	{
		return (int)mVecSkill[mLSkillIdx]->Type;
	}
	return -1;
}

int CD2PlayerSkillComponent::GetRSkillType()
{
	if (isValidIdx(mRSkillIdx))
	{
		return (int)mVecSkill[mRSkillIdx]->Type;
	}
	return -1;
}

void CD2PlayerSkillComponent::SetLSkill(const int idx)
{
	if (isValidIdx(mLSkillIdx))
	{
		mLSkillIdx = idx;
	}
}

void CD2PlayerSkillComponent::SetLSkill(const std::string& name)
{
	int idx = findSkillIdx(name);

	if (-1 == idx)
	{
		return;
	}

	mLSkillIdx = idx;
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

CGameObject* CD2PlayerSkillComponent::DoLSkill(const Vector3& startPos)
{
	if (isValidIdx(mLSkillIdx))
	{ 
		CGameObject* clone = mVecSkill[mLSkillIdx]->Object->Clone();
		clone->Enable(true);
		clone->Start();
		clone->SetWorldPos(startPos);
		return clone;
	}
	return nullptr;
}

CGameObject* CD2PlayerSkillComponent::DoRSkill(const Vector3& startPos)
{
	if (isValidIdx(mRSkillIdx))
	{ 
		CGameObject* clone = mVecSkill[mRSkillIdx]->Object->Clone();
		clone->Enable(true);
		clone->Start();
		clone->SetWorldPos(startPos);
		return clone;
	}
	return nullptr;
}

bool CD2PlayerSkillComponent::isValidIdx(const int idx)
{
	if (idx < 0 || idx >= mVecSkill.size())
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

	// TODO : Melee Attack As Default LSkill
	std::unordered_map<std::string, std::vector<std::string>*> table = data->GetTable();

	Skill* skill = nullptr;
	auto iter = table.begin();
	auto iterEnd = table.end();

	std::string outName;

	for (; iter != iterEnd; ++iter)
	{
		skill = new Skill;
		skill->Name = (*iter->second)[0];
		skill->Type = CD2Util::StringToAttackType((*iter->second)[1]);
		CGameObject* obj = mScene->LoadGameObjectFullPath(outName, (*iter->second)[2].c_str());

		if (!obj)
		{
			assert(false);
		}

		obj->SetWorldPos(-1000.f, -1000.f, 0.f);
		obj->Enable(false);

		skill->Object = obj;
		mVecSkill.push_back(skill);
	}

	CResourceManager::GetInst()->DeleteCSV(csvName);
	return true;
}

Skill* CD2PlayerSkillComponent::findSkill(const std::string& name)
{
	size_t size = mVecSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (mVecSkill[i]->Name == name)
		{
			return mVecSkill[i];
		}
	}
	return nullptr;
}

int CD2PlayerSkillComponent::findSkillIdx(const std::string& name)
{
	size_t size = mVecSkill.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (mVecSkill[i]->Name == name)
		{
			return (int)i;
		}
	}
	return -1;
}
