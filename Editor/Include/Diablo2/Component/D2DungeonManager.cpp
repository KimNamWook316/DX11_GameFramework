#include "D2DungeonManager.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"
#include "D2CharacterInfoComponent.h"
#include "D2ProcedualMapGenerator.h"
#include "../UI/D2FrameWidget.h"

DEFINITION_SINGLE(CD2DungeonManager)

CD2DungeonManager::CD2DungeonManager()
{
}

CD2DungeonManager::~CD2DungeonManager()
{
}

bool CD2DungeonManager::Init()
{
	return true;
}

void CD2DungeonManager::Start()
{
	if (!loadDungeon())
	{
		return;
	}

	mPlayer = CSceneManager::GetInst()->GetScene()->GetPlayerObj();
	mPlayerCharInfo = mPlayer->FindObjectComponentFromType<CD2CharacterInfoComponent>();
}

void CD2DungeonManager::BossDie()
{
}

void CD2DungeonManager::BossDead()
{
	mMapGenarator->OnClearDungeon();
}

void CD2DungeonManager::OnMonsterDead(int roomIdx)
{
	mMapGenarator->OnMonsterDead(roomIdx);
}

void CD2DungeonManager::OnMonsterDie(int exp)
{
	mPlayerCharInfo->SetExp(exp);
}

bool CD2DungeonManager::loadDungeon()
{
	bool result = false;
	std::string outName;
 	CGameObject* mapGenObj = CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, "D2ProcedualMapGenerator2.gobj");

	if (!mapGenObj)
	{
		return false;
	}

	mMapGenarator = mapGenObj->FindSceneComponentFromType<CD2ProcedualMapGenerator>();
	mMapGenarator->GenerateMap();

	result = CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, "UIManager.gobj");

	return result;
}
