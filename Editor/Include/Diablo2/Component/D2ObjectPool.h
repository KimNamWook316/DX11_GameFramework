#pragma once

#include "../D2Info.h"
#include "GameInfo.h"

class CD2ObjectPool
{
	friend class CD2ObjectPoolComponent;

public:
	bool Init();
	void Start();

public:
	class CGameObject* CloneSkillObj(const std::string& name);

private:
	void loadData();

	DECLARE_SINGLE(CD2ObjectPool)

private:
	std::unordered_map<std::string, CSharedPtr<class CGameObject>> mMapSkillObject;
};

