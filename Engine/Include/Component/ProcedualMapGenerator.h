#pragma once

#include "SceneComponent.h"
#include "TileMapComponent.h"

enum class eSplitDir
{
	None,
    Vertical,
    Horizontal
};

enum class eGenerationStep
{
	PartitionSpace,
	MakeRoom,
	ConnectRoom,
	Max
};

class CProcedualMapGenerator :
    public CSceneComponent
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
	void FindTileComponent();
	bool DoStep(eGenerationStep step);
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

	void AddRoom(struct RoomInfo* roomInfo)
	{
		mVecRoomInfo.push_back(roomInfo);
	}

	void AddSpace(class CSpace* space, const int level)
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

	eGenerationStep GetCurrentGenerationStepToDo() const;
	int GetCurrentGenerationStepDone() const;

private:
	void resetMapInfo();

private:
	std::vector<RoomInfo*> mVecRoomInfo;
	std::vector<eTileType> mVecMapInfo;
	std::vector<std::vector<class CSpace*>> mVecSpaceByLevel;
	CSharedPtr<CTileMapComponent> mTileMap;
	eTileShape meTileShape;
	Vector3 mTileSize;
	float mTileDiagonal;
	Matrix mMatIsoToWorld;
	CSpace* mSpaceTree;
	int mRoomSizeMin;
	int mMapCountX;
	int mMapCountY;
	int mPartitionLevel;
	bool mGenerationStep[(int)eGenerationStep::Max];
};

