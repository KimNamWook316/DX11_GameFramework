#pragma once

#include "GameInfo.h"
#include "../D2Info.h"

class CD2Room
{
	friend class CD2Space;
	friend class CD2ProcedualMapGenerator;

protected:
	CD2Room();
	~CD2Room();

public:
	bool GenerateRoomCollider();
	bool Spawn();
	void CreatePortal();
	void OnMonsterDead();

public:
	void OnCollideEnter(const CollisionResult& result);
	void OnCollideExit(const CollisionResult& result);

private:
	void spawnChest();

private:
	class CD2Space* mSpace;
	int mRoomIdx; // CProcedualMapGenerator의 MakeRoom에서 할당
	int mSizeX;
	int mSizeY;
	int mLBIdxX;
	int mLBIdxY;
	int mLBIdx;
	int mRTIdxX;
	int mRTIdxY;
	int mRTIdx;
	int mCenterIdxX;
	int mCenterIdxY;
	int mCenterIdx;
	std::list<int> mEntryIdxXList;
	std::list<int> mEtnryIdxYList;
	std::list<int> mEntryIdxList;
	std::list<class CGameObject*> mRoomColliderObjList;
	int mPlayerEnterCount;
	int mPlayerExitCount;
	eD2RoomSpawnType meSpawnType;
	std::list<class CGameObject*> mMonsterList;
	int mMonsterCount;
};

