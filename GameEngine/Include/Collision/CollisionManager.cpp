#include "CollisionManager.h"
#include "../Resource/ResourceManager.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{

}

CCollisionManager::~CCollisionManager()
{
	auto iter = mMapProfile.begin();
	auto iterEnd = mMapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CCollisionManager::Init()
{
	// Default Profile
	CreateProfile("Object", eCollisionChannel::Object, true);
	CreateProfile("Player", eCollisionChannel::Player, true);
	CreateProfile("Monster", eCollisionChannel::Monster, true);
	CreateProfile("PlayerAttack", eCollisionChannel::PlayerAttack, true);
	CreateProfile("MonsterAttack", eCollisionChannel::MonsterAttack, true);

	// Default Proflie Setting
	SetCollisionState("Player", eCollisionChannel::Player, eCollisionInteraction::Ignore);
	SetCollisionState("Player", eCollisionChannel::PlayerAttack, eCollisionInteraction::Ignore);

	SetCollisionState("Monster", eCollisionChannel::Monster, eCollisionInteraction::Ignore);
	SetCollisionState("Monster", eCollisionChannel::MonsterAttack, eCollisionInteraction::Ignore);

	SetCollisionState("PlayerAttack", eCollisionChannel::Player, eCollisionInteraction::Ignore);
	SetCollisionState("PlayerAttack", eCollisionChannel::PlayerAttack, eCollisionInteraction::Ignore);
	SetCollisionState("PlayerAttack", eCollisionChannel::MonsterAttack, eCollisionInteraction::Ignore);

	SetCollisionState("MonsterAttack", eCollisionChannel::Monster, eCollisionInteraction::Ignore);
	SetCollisionState("MonsterAttack", eCollisionChannel::PlayerAttack, eCollisionInteraction::Ignore);
	SetCollisionState("MonsterAttack", eCollisionChannel::MonsterAttack, eCollisionInteraction::Ignore);

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& name, eCollisionChannel eChannel, bool bEnable, eCollisionInteraction eState)
{
	CollisionProfile* profile = FindProfile(name);

	if (profile)
	{
		return false;
	}

	profile = new CollisionProfile;
	profile->Name = name;
	profile->Channel = eChannel;
	profile->bCollisionEnable = bEnable;
	profile->vecInteraction.resize((int)eCollisionChannel::Max); // 모든 프로파일에 상태 가지도록 세팅

	for (int i = 0; i < (int)eCollisionChannel::Max; ++i)
	{
		profile->vecInteraction[i] = eState;
	}

	mMapProfile.insert(std::make_pair(name, profile));

	return true;
}

bool CCollisionManager::SetCollisionState(const std::string& name, eCollisionChannel eChannel, eCollisionInteraction state)
{
	CollisionProfile* profile = FindProfile(name);

	if (!profile)
	{
		return false;
	}

	profile->vecInteraction[(int)eChannel] = state;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& name)
{
	auto iter = mMapProfile.find(name);

	if (iter == mMapProfile.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CCollisionManager::GetProfileNames(std::vector<std::string>& outNames)
{
	auto iter = mMapProfile.begin();
	auto iterEnd = mMapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		outNames.push_back(iter->first);
	}
}

bool CCollisionManager::MakeCSV()
{
	if (!CResourceManager::GetInst()->CreateCSV("CollisionProfileInfo"))
	{
		return false;
	}

	CExcelData* csv = CResourceManager::GetInst()->FindCSV("CollisionProfileInfo");
	csv->Clear();

	csv->AddLabel("Channel");

	for (int i = 0; i < (int)eCollisionChannel::Max; ++i)
	{
		csv->AddLabel(CUtil::CollisionChannelToString((eCollisionChannel)i));
	}

	std::vector<std::string> profileData;
	auto iter = mMapProfile.begin();
	auto iterEnd = mMapProfile.end();
	for (; iter != iterEnd; ++iter)
	{
		profileData.push_back(CUtil::CollisionChannelToString(iter->second->Channel));
		for (int i = 0; i < (int)eCollisionChannel::Max; ++i)
		{
			std::string bCollision;
			profileData.push_back(CUtil::CollsionInteractionToString(iter->second->vecInteraction[i]));
		}

		csv->SetData(iter->first, profileData);
		profileData.clear();
	}

	return true;
}

bool CCollisionManager::LoadProfile(const char* fileName, const std::string& path)
{
	auto iter = mMapProfile.begin();
	auto iterEnd = mMapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mMapProfile.clear();

	std::string outName;
	if (!CResourceManager::GetInst()->LoadCSV(outName, fileName))
	{
		return false;
	}

	std::unordered_map<std::string, std::vector<std::string>*> table = CResourceManager::GetInst()->FindCSV(outName)->GetTable();

	auto iterCSV = table.begin();
	auto iterEndCSV = table.end();

	for (; iterCSV != iterEndCSV; ++iterCSV)
	{
		CreateProfile(iterCSV->first, CUtil::StringToCollisionChannel((*iterCSV->second)[0]), true);

		for (int i = 1; i <= 15; ++i)
		{
			SetCollisionState(iterCSV->first, (eCollisionChannel)(i - 1), CUtil::StringToCollisionInteraction((*iterCSV->second)[i]));
		}
	}

	CResourceManager::GetInst()->DeleteCSV(outName);
}

