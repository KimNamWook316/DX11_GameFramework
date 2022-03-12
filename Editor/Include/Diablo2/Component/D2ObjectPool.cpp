#include "D2ObjectPool.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "D2SkillObject.h"

DEFINITION_SINGLE(CD2ObjectPool)

CD2ObjectPool::CD2ObjectPool()
{
}

CD2ObjectPool::~CD2ObjectPool()
{
}

bool CD2ObjectPool::Init()
{
	return true;
}

void CD2ObjectPool::Start()
{
	loadData();
}

CGameObject* CD2ObjectPool::CloneSkillObj(const std::string& name)
{
	auto iter = mMapSkillObject.find(name);

	if (iter == mMapSkillObject.end())
	{
		return nullptr;
	}

	return iter->second->Clone();
}

void CD2ObjectPool::loadData()
{
	std::string outName;
	CResourceManager::GetInst()->LoadCSV(outName, "ObjectList.csv");

	std::unordered_map<std::string, std::vector<std::string>*> table = CResourceManager::GetInst()->FindCSV(outName)->GetTable();

	auto iter = table.begin();
	auto iterEnd = table.end();

	for (; iter != iterEnd; ++iter)
	{
		std::string outName;
		CGameObject* obj = CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, (*iter->second)[1].c_str());

		if ((*iter->second)[2] == "Skill")
		{
			mMapSkillObject.insert(std::make_pair((*iter->second)[0], obj));
			static_cast<CD2SkillObject*>(obj->FindComponent("Script"))->SetInfo((*iter->second)[0]);
		}

		obj->Start();
		obj->Enable(false);
		obj->SetWorldPos(-1000.f, -1000.f, 0.f);
	}
}
