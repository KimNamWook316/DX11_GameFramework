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

CGameObject* CD2ObjectPool::ActiveEffect(const std::string& name, const Vector3& pos)
{
	auto iter = mMapEffectObject.find(name);

	if (iter == mMapEffectObject.end())
	{
		return nullptr;
	}

	CGameObject* effect = iter->second.InActiveList.front();
	iter->second.InActiveList.pop_front();
	effect->Enable(true);
	effect->Start();
	effect->SetWorldPos(pos.x, pos.y - 1.f, pos.z);

	iter->second.ActiveList.push_back(effect);
	
	return effect;
}

void CD2ObjectPool::ReturnEffect(const std::string& name, CGameObject* obj)
{
	auto iter = mMapEffectObject.find(name);

	if (iter == mMapEffectObject.end())
	{
		return;
	}

	auto iterObj = iter->second.ActiveList.begin();
	auto iterObjEnd = iter->second.ActiveList.end();

	for (; iterObj != iterObjEnd; ++iterObj)
	{
		if (*iterObj == obj)
		{
			iter->second.ActiveList.erase(iterObj);
			break;
		}
	}

	obj->Enable(false);
	obj->SetWorldPos(-1000.f, -1000.f, 0.f);
	iter->second.InActiveList.push_back(obj);
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
		if ((*iter->second)[2] == "Skill")
		{
			std::string outName;
			CGameObject* obj = CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, (*iter->second)[1].c_str());
			mMapSkillObject.insert(std::make_pair((*iter->second)[0], obj));
			static_cast<CD2SkillObject*>(obj->FindComponent("Script"))->SetInfo((*iter->second)[0]);

			obj->Start();
			obj->Enable(false);
			obj->SetWorldPos(-1000.f, -1000.f, 0.f);
		}

		else if ((*iter->second)[2] == "Effect")
		{
			std::string outName;
			CGameObject* obj = CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, (*iter->second)[1].c_str());

			CGameObject* clone = nullptr;
			Pool pool = {};

			// 50개 복사해서 가지고 있는다.
			for (int i = 0; i < 50; ++i)
			{
				clone = obj->Clone();
				clone->Start();
				clone->Enable(false);
				clone->SetWorldPos(-1000.f, -1000.f, 0.f);
				pool.InActiveList.push_back(clone);
			}

			// 원본은 삭제
			obj->Destroy();

			mMapEffectObject.insert(std::make_pair((*iter->second)[0], pool));
		}

	}
}
