#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
public:
	bool Init();
	bool CreateProfile(const std::string& name, eCollisionChannel eChannel,
		bool bEnable, eCollisionInteraction eState = eCollisionInteraction::Collision);
	bool SetCollisionState(const std::string& name, eCollisionChannel eChannel,
		eCollisionInteraction state);
	CollisionProfile* FindProfile(const std::string& name);
	void GetProfileNames(std::vector<std::string>& outNames);

	DECLARE_SINGLE(CCollisionManager)

private:
	std::unordered_map<std::string, CollisionProfile*> mMapProfile;
};

