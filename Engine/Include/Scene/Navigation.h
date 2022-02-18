#pragma once

#include "../Component/TileMapComponent.h"

struct NavNode
{
	eNavNodeType eNodeType;
	eTileType eTileType;
	Vector3 Pos;
	Vector2 Size;
	Vector3 Center;
	int indexX;
	int indexY;
	int index;
	float Cost;
	float Dist;
	float Total;

	NavNode() :
		eNodeType(eNavNodeType::None),
		eTileType(eTileType::Normal),
		indexX(-1),
		indexY(-1),
		index(-1),
		Cost(0.f),
		Dist(0.f),
		Total(0.f)
	{
	}
};

class CNavigation
{
	friend class CNavigationThread;

private:
	CNavigation();
	~CNavigation();

public:
	void CreateNavigationNodes(class CTileMapComponent* tileMap);
	bool FindPath(const Vector3& start, const Vector3& end, std::vector<Vector3>& outVecPath);

private:
	eTileShape meNodeShape;
	std::vector<NavNode*> mVecNode;
	int mCountX;
	int mCountY;
	Vector2 mTileSize;
	CSharedPtr<CTileMapComponent> mTileMap;
};

