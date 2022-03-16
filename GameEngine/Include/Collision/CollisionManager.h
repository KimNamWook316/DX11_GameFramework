#pragma once

#include "../GameInfo.h"
#include "../Resource/Excel/ExcelData.h"

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
	bool LoadProfile(const char* fileName, const std::string& path = EXCEL_PATH);

public:
	bool MakeCSV();

	DECLARE_SINGLE(CCollisionManager)

private:
	std::unordered_map<std::string, CollisionProfile*> mMapProfile;
};

