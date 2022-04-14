#pragma once

#include "GameInfo.h"

class CD2DungeonManager
{
public:
	bool Init();
	void Start();

public:
	void BossDie();
	void BossDead();

public:
	void OnMonsterDead(int roomIdx);
	void OnMonsterDie(int exp);

private:
	bool loadDungeon();

	DECLARE_SINGLE(CD2DungeonManager)

private:
	class CD2ProcedualMapGenerator* mMapGenarator;
	class CGameObject* mPlayer;
	class CD2CharacterInfoComponent* mPlayerCharInfo;
};

