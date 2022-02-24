#pragma once

#include "ObjectComponent.h"
#include "Space.h"

enum class eSplitDir
{
	None,
    Vertical,
    Horizontal
};

// eTileType과 인덱스 맞아야 함
enum class eMapObject
{
	TileNormal,
	TileWall,
	TileCornerTop,
	TileCornerRight,
	TileCornerBottom,
	TileCornerLeft,
	TileWallTopRight,
	TileWallBottomRight,
	TileWallLeftBottom,
	TileWallLeftTop,
	TileEntryRightTopLeft,
	TileEntryRightTopRigt,
	TileEntryLeftTopLeft,
	TileEntryLeftTopRight,
	TileEntryRightBottomLeft,
	TileEntryRightBottomRight,
	TileEntryLeftBottomLeft,
	TileEntryLeftBottomRight,
	Max,
};

class CProcedualMapGenerator :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CProcedualMapGenerator();
	CProcedualMapGenerator(const CProcedualMapGenerator& com);
	virtual ~CProcedualMapGenerator();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CProcedualMapGenerator* Clone() override;

public:
	bool GenerateMap();
	bool PartitionSpace();
	bool MakeRoom();
	bool ConnectRoom();
	bool PassToTileMap();

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

	void AddRoom(RoomInfo* roomInfo)
	{
		mVecRoomInfo.push_back(roomInfo);
	}

	void AddSpace(CSpace* space, const int level)
	{
		mVecSpaceByLevel[level].push_back(space);
	}

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

private:
	std::vector<RoomInfo*> mVecRoomInfo;
	std::vector<eMapObject> mVecMapInfo;
	std::vector<std::vector<CSpace*>> mVecSpaceByLevel;
	CSharedPtr<class CTileMapComponent*> mTileMap;
	CSpace* mSpaceTree;
	int mRoomSizeMin;
	int mMapCountX;
	int mMapCountY;
	int mPartitionLevel;
};

