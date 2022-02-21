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
	bool FindPath(const Vector3& start, const Vector3& end, std::vector<Vector3>& outVecPath);

private:
	void findNode(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getCorner(eNodeDir dir, NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	static bool sortNode(NavNode* src, NavNode* dst);

private:
	NavNode* getNodeTop(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeRightTop(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeRight(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeRightBottom(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeBottom(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeLeftBottom(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeLeft(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);
	NavNode* getNodeLeftTop(NavNode* node, NavNode* endNode, const Vector3& end, std::vector<Vector3>& outVecPath);

private:
	eTileShape meNodeShape;
	std::vector<NavNode*> mVecNode;
	int mCountX;
	int mCountY;
	Vector2 mTileSize;
	CSharedPtr<CTileMapComponent> mTileMap;
	std::vector<NavNode*> mVecOpen;
	std::vector<NavNode*> mVecUseNode;
};

