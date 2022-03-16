#pragma once

#include "../Component/TileMapComponent.h"

enum class eNodeDir
{
	T, 
	RT,
	R,
	RB,
	B,
	LB,
	L,
	LT,
	Max
};

struct NavNode
{
	NavNode* Parent;
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
	std::list<eNodeDir> SearchDirList;

	NavNode() :
		eNodeType(eNavNodeType::None),
		eTileType(eTileType::Normal),
		indexX(-1),
		indexY(-1),
		index(-1),
		Cost(FLT_MAX),
		Dist(FLT_MAX),
		Total(FLT_MAX),
		Parent(nullptr)
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
	void ChangeNode(const int idx, eTileType eType);
	bool FindPath(const Vector3& start, const Vector3& end, std::list<Vector3>& outListPath);

private:
	bool findNode(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath);
	NavNode* getCorner(eNodeDir dir, NavNode* node, NavNode* endNode, const Vector3& end);
	static bool sortNode(NavNode* src, NavNode* dst);
	void addDir(eNodeDir dir, NavNode* node);

private:
	NavNode* getNodeTop(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeRightTop(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeRight(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeRightBottom(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeBottom(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeLeftBottom(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeLeft(NavNode* node, NavNode* endNode, const Vector3& end);
	NavNode* getNodeLeftTop(NavNode* node, NavNode* endNode, const Vector3& end);

private:
	eTileShape meNodeShape;
	std::vector<NavNode*> mVecNode;
	int mCountX;
	int mCountY;
	Vector2 mTileSize;
	CSharedPtr<CTileMapComponent> mTileMap;
	std::vector<NavNode*> mVecOpen;
	std::vector<NavNode*> mVecUseNode;
	CRITICAL_SECTION mCrt;
};

