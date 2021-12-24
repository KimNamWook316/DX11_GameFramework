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
	SetCollisionState("Player", eCollisionChannel::Player, eCollisionState::Ignore);
	SetCollisionState("Player", eCollisionChannel::PlayerAttack, eCollisionState::Ignore);

	SetCollisionState("Monster", eCollisionChannel::Monster, eCollisionState::Ignore);
	SetCollisionState("Monster", eCollisionChannel::MonsterAttack, eCollisionState::Ignore);

	SetCollisionState("PlayerAttack", eCollisionChannel::Player, eCollisionState::Ignore);
	SetCollisionState("PlayerAttack", eCollisionChannel::PlayerAttack, eCollisionState::Ignore);
	SetCollisionState("PlayerAttack", eCollisionChannel::MonsterAttack, eCollisionState::Ignore);

	SetCollisionState("MonsterAttack", eCollisionChannel::Monster, eCollisionState::Ignore);
	SetCollisionState("MonsterAttack", eCollisionChannel::PlayerAttack, eCollisionState::Ignore);
	SetCollisionState("MonsterAttack", eCollisionChannel::MonsterAttack, eCollisionState::Ignore);

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& name, eCollisionChannel eChannel, bool bEnable, eCollisionState eState)
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
	profile->vecState.resize((int)eCollisionChannel::Max); // 모든 프로파일에 상태 가지도록 세팅

	for (int i = 0; i < (int)eCollisionChannel::Max; ++i)
	{
		profile->vecState[i] = eState;
	}

	mMapProfile.insert(std::make_pair(name, profile));

	return true;
}

bool CCollisionManager::SetCollisionState(const std::string& name, eCollisionChannel eChannel, eCollisionState state)
{
	CollisionProfile* profile = FindProfile(name);

	if (!profile)
	{
		return false;
	}

	profile->vecState[(int)eChannel] = state;

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& name)
{
	auto iter = mMapProfile.find(name);

	if (iter == mMapProfile.end())
	{
		assert(false);
		return nullptr;
	}

	return iter->second;
}
