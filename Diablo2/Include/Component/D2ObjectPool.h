#pragma once

#include "../D2Info.h"
#include "GameInfo.h"

struct Pool
{
	std::list<class CGameObject*> InActiveList;
	std::list<class CGameObject*> ActiveList;
};

class CD2ObjectPool
{
	friend class CD2ObjectPoolComponent;

public:
	bool Init();
	void Start();

public:
	class CGameObject* CloneSkillObj(const std::string& name);

	class CGameObject* ActiveEffect(const std::string& name, const Vector3& pos);
	void ReturnEffect(const std::string& name, class CGameObject* obj);

	class CGameObject* ActiveInteractionObject(const std::string& name, const Vector3& pos);
	void ReturnInteractionObject(const std::string& name, class CGameObject* obj);

private:
	void loadData();

	DECLARE_SINGLE(CD2ObjectPool)

private:
	std::unordered_map<std::string, class CGameObject*> mMapSkillObject;
	std::unordered_map<std::string, Pool> mMapEffectObject;
	std::unordered_map<std::string, Pool> mMapInteractionObject;
};

