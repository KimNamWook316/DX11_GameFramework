#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
public:
	bool Init();
	bool CreateProfile(const std::string& name, eCollisionChannel eChannel,
		bool bEnable, eCollisionState eState = eCollisionState::Collision);
	bool SetCollisionState(const std::string& name, eCollisionChannel eChannel,
		eCollisionState state);
	CollisionProfile* FindProfile(const std::string& name);

	DECLARE_SINGLE(CCollisionManager)

private:
	std::unordered_map<std::string, CollisionProfile*> mMapProfile;
};

