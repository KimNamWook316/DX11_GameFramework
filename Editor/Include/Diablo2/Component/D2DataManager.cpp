#include "D2DataManager.h":
#include "Resource/ResourceManager.h"
#include "../D2Util.h"

DEFINITION_SINGLE(CD2DataManager);

CD2DataManager::CD2DataManager()
{
}

CD2DataManager::~CD2DataManager()
{
	{
		auto iter = mMapCharInfo.begin();
		auto iterEnd = mMapCharInfo.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		mMapCharInfo.clear();
	}
}

bool CD2DataManager::Init()
{
	if (!loadD2CharInfo())
	{
		return false;
	}

	if (!loadD2SkillInfo())
	{
		return false;
	}

	return true;
}

void CD2DataManager::Start()
{
	if (mMapCharInfo.empty())
	{
		if (!loadD2CharInfo())
		{
			return;
		}
	}
	
	if (mMapSkillInfo.empty())
	{
		if (!loadD2SkillInfo())
		{
			return;
		}
	}
}

D2CharInfo* CD2DataManager::FindCharcterInfo(const std::string& name)
{
	auto find = mMapCharInfo.find(name);

	if (find == mMapCharInfo.end())
	{
		return nullptr;
	}
	return find->second;
}

void CD2DataManager::GetCharacterNames(std::vector<std::string>& charNames)
{
	auto iter = mMapCharInfo.begin();
	auto iterEnd = mMapCharInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		charNames.push_back(iter->second->Name);
	}
}

D2SkillInfo* CD2DataManager::FindSkillInfo(const std::string& name)
{
	auto find = mMapSkillInfo.find(name);

	if (find == mMapSkillInfo.end())
	{
		return nullptr;
	}
	return find->second;
}

void CD2DataManager::GetSkillNames(std::vector<std::string>& skillNames)
{
	auto iter = mMapSkillInfo.begin();
	auto iterEnd = mMapSkillInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		skillNames.push_back(iter->second->Name);
	}
}

bool CD2DataManager::loadD2CharInfo()
{
	std::string outName;
	if (!CResourceManager::GetInst()->LoadCSV(outName, "D2CharInfo.csv"))
	{
		return false;
	}

	CExcelData* csv = CResourceManager::GetInst()->FindCSV(outName);

	std::unordered_map<std::string, std::vector<std::string>*> table = csv->GetTable();

	D2CharInfo* info = nullptr;

	auto iter = table.begin();
	auto iterEnd = table.end();

	for (; iter != iterEnd; ++iter)
	{
		info = new D2CharInfo;
		info->Dir = Vector2(0.f, -1.f);
		info->eSpriteDir = eD2SpriteDir::S;
		info->Speed = 0.f;
		info->Name = (*iter->second)[0];
		info->MaxSpeed = std::stof((*iter->second)[1]);
		info->MaxHp = std::stof((*iter->second)[2]);
		info->Hp = info->MaxHp;
		info->MaxCCTime = std::stof((*iter->second)[3]);

		mMapCharInfo.insert(std::make_pair(info->Name, info));
	}

	return true;
}

bool CD2DataManager::loadD2SkillInfo()
{
	std::string outName;
	if (!CResourceManager::GetInst()->LoadCSV(outName, "D2SkillInfo.csv"))
	{
		return false;
	}

	CExcelData* csv = CResourceManager::GetInst()->FindCSV(outName);

	std::unordered_map<std::string, std::vector<std::string>*> table = csv->GetTable();

	D2SkillInfo* info = nullptr;

	auto iter = table.begin();
	auto iterEnd = table.end();

	for (; iter != iterEnd; ++iter)
	{
		info = new D2SkillInfo;
		info->Name = (*iter->second)[0];
		info->Damage = std::stof((*iter->second)[1]);
		info->Speed = std::stof((*iter->second)[2]);
		info->eElementType = CD2Util::StringToElementType((*iter->second)[3]);
		info->LifeTime = std::stof((*iter->second)[4]);
		mMapSkillInfo.insert(std::make_pair(info->Name, info));
	}

	return true;
}
