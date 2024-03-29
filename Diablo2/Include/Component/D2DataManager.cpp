#include "D2DataManager.h":
#include "Resource/ResourceManager.h"
#include "Resource/Texture/Texture.h"
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

	{
		auto iter = mMapSkillInfo.begin();
		auto iterEnd = mMapSkillInfo.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		mMapSkillInfo.clear();
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

CTexture* CD2DataManager::GetSkillIconTexture(const std::string& skilName)
{
	return CResourceManager::GetInst()->FindTexture(skilName + "Icon");
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
		info->MaxMp = std::stof((*iter->second)[4]);
		info->Mp = info->MaxMp;
		info->MeleeBonus = std::stof((*iter->second)[5]);
		info->MagicBonus = std::stof((*iter->second)[6]);
		info->MaxExp = std::stof((*iter->second)[7]);
		info->Exp = 0;
		info->MaxLevel = std::stof((*iter->second)[8]);
		info->Level = 1;

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

		std::string iconPath = (*iter->second)[5];
		if (iconPath != "FALSE")
		{
			char buf[512] = {};
			strcpy_s(buf, iconPath.c_str());
			TCHAR bufConvert[512] = {};
			int length = MultiByteToWideChar(CP_ACP, 0, buf, -1, nullptr, 0);
			MultiByteToWideChar(CP_ACP, 0, buf, length, bufConvert, length);

			CResourceManager::GetInst()->LoadTexture(info->Name + "Icon", bufConvert);
		}
	}

	return true;
}
