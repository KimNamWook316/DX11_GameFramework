#include "CollisionManager.h"

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

	makeCSVData();

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
	
	addCSVData(*profile);

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

void CCollisionManager::addCSVData(const CollisionProfile& profile)
{
	std::vector<std::string> row;
	row.push_back(profile.Name);
	row.push_back(CUtil::CollisionChannelToString(profile.Channel));

	for (size_t i = 0; i < (int)eCollisionChannel::Max; ++i)
	{
		row.push_back(CUtil::CollsionInteractionToString(profile.vecInteraction[i]));
	}

	mCSVData.push_back(row);
}

void CCollisionManager::makeCSVData()
{
	mCSVData.clear();

	std::vector<std::string> labels;
	labels.push_back("ProfileName");
	labels.push_back("Channel");

	for (size_t i = 0; i < (int)eCollisionChannel::Max; ++i)
	{
		labels.push_back(CUtil::CollisionChannelToString((eCollisionChannel)i));
	}

	mCSVData.push_back(labels);

	auto iter = mMapProfile.begin();
	auto iterEnd = mMapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		std::vector<std::string> row;
		row.push_back(iter->first);
		row.push_back(CUtil::CollisionChannelToString(iter->second->Channel));

		for (size_t i = 0; i < (int)eCollisionChannel::Max; ++i)
		{
			row.push_back(CUtil::CollsionInteractionToString(iter->second->vecInteraction[i]));
		}

		mCSVData.push_back(row);
	}
}
