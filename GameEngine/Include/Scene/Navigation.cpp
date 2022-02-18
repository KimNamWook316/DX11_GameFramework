#include "Navigation.h"
#include "../Component/TileMapComponent.h"

CNavigation::CNavigation()
{
}

CNavigation::~CNavigation()
{
}

void CNavigation::CreateNavigationNodes(CTileMapComponent* tileMap)
{
	mTileMap = tileMap;

	meNodeShape = tileMap->GetTileShape();
	mCountX = tileMap->GetTileCountX();
	mCountY = tileMap->GetTileCountY();
	mTileSize = tileMap->GetTileSize();

	int count = mCountX * mCountY;

	for (int i = 0; i < count; ++i)
	{
		NavNode* node = new NavNode;
		node->eTileType = tileMap->GetTile(i)->GetTileType();
		node->Pos = tileMap->GetTile(i)->GetPos();
		node->Size = mTileSize;
		node->indexX = tileMap->GetTile(i)->GetIndexX();
		node->indexY = tileMap->GetTile(i)->GetIndexY();
		node->index = tileMap->GetTile(i)->GetIndex();

		mVecNode.push_back(node);
	}
}

bool CNavigation::FindPath(const Vector3& start, const Vector3& end, std::vector<Vector3>& outVecPath)
{
	outVecPath.clear();

	int startIndex = mTileMap->GetTileIndex(start);

	if (-1 == startIndex)
	{
		return false;
	}

	int endIndex = mTileMap->GetTileIndex(end);

	if (-1 == endIndex)
	{
		return false;
	}

	return true;
}
