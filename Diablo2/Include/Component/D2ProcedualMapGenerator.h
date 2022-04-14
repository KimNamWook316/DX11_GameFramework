#pragma once

#include "Component/SceneComponent.h"
#include "Component/TileMapComponent.h"

enum class eD2ProcedualGenerationStep
{
    PartitionSpace,
    MakeRoom,
    ConnectRoom,
    CreatePathFindInfo,
    GenerateWall,
    Spawn,
    Max
};

class CD2ProcedualMapGenerator :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CD2ProcedualMapGenerator();
	CD2ProcedualMapGenerator(const CD2ProcedualMapGenerator& com);
	virtual ~CD2ProcedualMapGenerator();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CD2ProcedualMapGenerator* Clone() override;
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
	bool GenerateMap();
	bool CreateTileComponent();
	bool CreateWallComponent();
	bool PartitionSpace();
	bool PartitionSpaceDebug();
	bool MakeRoom();
	bool MakeRoomDebug();
	bool ConnectRoom();
	bool ConnectRoomDebug();
	bool GenerateTile();
	bool GenerateWall();
	bool GenerateWallDebug();
	bool CreatePathFindInfo();
	bool CreatePathFindInfoDebug();
	bool Spawn();
	bool SpawnDebug();
	void OnClearDungeon();
	void OnMonsterDead(int roomIdx);

public:
	bool SetMapCount(const int x, const int y)
	{
		if (x < 7 || y < 7)
		{
			assert(false);
			return false;
		}
		mMapCountX = x;
		mMapCountY = y;
	}

	bool SetPartitionLevel(const int level)
	{
		if (level < 1)
		{
			assert(false);
			return false;
		}
		mPartitionLevel = level;
	}

	void AddRoom(class CD2Room* roomInfo)
	{
		mVecRoom.push_back(roomInfo);
	}

	void AddSpace(class CD2Space* space, const int level)
	{
		mVecSpaceByLevel[level].push_back(space);
	}

	void SetTileShape(const eTileShape shape)
	{
		meTileShape = shape;
	}

	void SetTileSize(const Vector3& size)
	{
		mTileSize = size;
		mTileDiagonal = sqrt(((mTileSize.x / 2.f) * (mTileSize.x / 2.f)) + ((mTileSize.y / 2.f) * (mTileSize.y / 2.f)));
	}

	void SetTileSet(class CTileSet* tileSet)
	{
		mTileSet = tileSet;
	}

	void SetWallTileSet(class CTileSet* tileSet)
	{
		mWallTileSet = tileSet;
	}

	void SetRoomSizeMin(const int roomSize)
	{
		mRoomSizeMin = roomSize;
	}

	void SetTileType(const int idx, eTileType eMapObj);
	void SetTileType(const int x, const int y, eTileType eMapObj);

public:
	int GetMapCountX() const
	{
		return mMapCountX;
	}

	int GetMapCountY() const
	{
		return mMapCountY;
	}

	int GetRoomSizeMin() const
	{
		return mRoomSizeMin;
	}

	eTileType GetTileType(const int idx) const
	{
		if (idx < 0 || idx >= mVecMapInfo.size())
		{
			return (eTileType)-1;
		}
		return mVecMapInfo[idx];
	}

	eTileType GetTileType(const int x, const int y) const
	{
		return GetTileType(y * mMapCountX + x);
	}

	eTileShape GetTileShape() const
	{
		return meTileShape;
	}

	const Vector3& GetTileSize() const
	{
		return mTileSize;
	}

	const Matrix& GetMatIsoToWorld() const
	{
		return mMatIsoToWorld;
	}

	float GetTileDiagonal() const
	{
		return mTileDiagonal;
	}

	CTileMapComponent* GetTileMapComponent() const
	{
		return mTileMap;
	}

	CWallComponent* GetWallComponent() const
	{
		return mTileMap->GetWallComponent();
	}

	CTileSet* GetTileSet() const
	{
		return mTileSet;
	}

	CTileSet* GetWallTileSet() const
	{
		return mWallTileSet;
	}

	int GetPartitionLevel() const
	{
		return mPartitionLevel;
	}

	eD2ProcedualGenerationStep GetCurGenerationStep() const
	{
		return meCurGenerationStep;
	}

private:
	void resetMapInfo();

private:
	std::vector<class CD2Room*> mVecRoom;
	class CD2Room* mBossRoom;
	std::vector<eTileType> mVecMapInfo;
	std::vector<std::vector<class CD2Space*>> mVecSpaceByLevel;
	CSharedPtr<CTileMapComponent> mTileMap;
	eTileShape meTileShape;
	Vector3 mTileSize;
	float mTileDiagonal;
	Matrix mMatIsoToWorld;
	CD2Space* mSpaceTree;
	int mRoomSizeMin;
	int mMapCountX;
	int mMapCountY;
	int mPartitionLevel;

	CSharedPtr<class CTileSet> mTileSet;
	CSharedPtr<class CTileSet> mWallTileSet;

	eD2ProcedualGenerationStep meCurGenerationStep;
};

