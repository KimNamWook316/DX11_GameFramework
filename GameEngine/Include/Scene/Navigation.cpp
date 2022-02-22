#include "Navigation.h"
#include "../Component/TileMapComponent.h"
#include "../Component/Tile.h"

CNavigation::CNavigation()	:
	mCountX(0),
	mCountY(0),
	meNodeShape(eTileShape::Rect)
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
		node->Center = node->Pos + Vector3(node->Size.x, node->Size.y, 0.f) * 0.5f;
		node->indexX = tileMap->GetTile(i)->GetIndexX();
		node->indexY = tileMap->GetTile(i)->GetIndexY();
		node->index = tileMap->GetTile(i)->GetIndex();

		mVecNode.push_back(node);
	}
}

bool CNavigation::FindPath(const Vector3& start, const Vector3& end, std::list<Vector3>& outListPath)
{
	outListPath.clear();

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

	// 이전 목록 초기화
	size_t size = mVecUseNode.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecUseNode[i]->eNodeType = eNavNodeType::None;
		mVecUseNode[i]->Cost = FLT_MAX;
		mVecUseNode[i]->Dist = FLT_MAX;
		mVecUseNode[i]->Total = FLT_MAX;
		mVecUseNode[i]->Parent = nullptr;
	}

	NavNode* startNode = mVecNode[startIndex];
	NavNode* endNode = mVecNode[endIndex];
	
	// 목적지가 벽이면 길 찾지 않음
	if (eTileType::Wall == endNode->eTileType)
	{
		return false;
	}
	
	// 목적지와 출발지점이 같은 경우
	if (startNode == endNode)
	{
		outListPath.push_back(end);
		return true;
	}

	startNode->eNodeType = eNavNodeType::Open;
	startNode->Cost = 0.f;
	startNode->Dist = startNode->Center.Distance(end);
	startNode->Total = startNode->Dist;

	mVecOpen.push_back(startNode);
	mVecUseNode.push_back(startNode);

	while (!mVecOpen.empty())
	{
		// total 내림차순으로 정렬되어 있는 열린 목록 맨 뒤의 원소를 빼냄
		// 비용이 가장 적은 순서로 정렬된 우선순위 큐
		NavNode* node = mVecOpen.back();
		mVecOpen.pop_back();

		node->eNodeType = eNavNodeType::Closed;

		if (findNode(node, endNode, end, outListPath))
		{
			break;
		}

		// 열린목록 정렬
		if (!mVecOpen.empty())
		{
			qsort(&mVecOpen[0], mVecOpen.size(), sizeof(NavNode*), CNavigation::sortNode);
		}
	}

	bool bFound = mVecOpen.empty() ? false : true;

	return bFound;
}

bool CNavigation::findNode(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	for (int i = 0; i < (int)eNodeDir::Max; ++i)
	{
		NavNode* corner = getCorner((eNodeDir)i, node, endNode, end, outListPath);

		if (!corner)
		{
			continue;
		}

		// 찾은 노드가 도착 노드라면 경로 생성
		if (endNode == corner)
		{
			outListPath.push_front(end);
			
			NavNode* pathNode = node;

			while (pathNode)
			{
				outListPath.push_front(pathNode->Center);
				pathNode = pathNode->Parent;
			}

			return true;
		}

		// 비용 계산
		float cost = 0.f;

		if (eTileShape::Rect == meNodeShape)
		{
			switch (eNodeDir(i))
			{
			case eNodeDir::T:
			case eNodeDir::B:
				cost = node->Cost + abs(node->Center.y - corner->Center.y);
				break;
			case eNodeDir::R:
			case eNodeDir::L:
				cost = node->Cost + abs(node->Center.x - corner->Center.x);
				break;
			case eNodeDir::RT:
			case eNodeDir::RB:
			case eNodeDir::LB:
			case eNodeDir::LT:
				cost = node->Cost + node->Center.Distance(corner->Center);
				break;
			default:
				assert(false);
				return false;
			}
		}
		else if (eTileShape::Rhombus == meNodeShape)
		{
			switch (eNodeDir(i))
			{
			case eNodeDir::T:
			case eNodeDir::B:
			case eNodeDir::R:
			case eNodeDir::L:
				cost = node->Cost + node->Center.Distance(corner->Center);
				break;
			case eNodeDir::RT:
			case eNodeDir::LB:
				cost = node->Cost + abs(node->Center.x - corner->Center.x);
			case eNodeDir::RB:
			case eNodeDir::LT:
				cost = node->Cost + abs(node->Center.y - corner->Center.y);
				break;
			default:
				assert(false);
				return false;
			}
		}

		// 찾은 노드가 이미 열린 목록에 들어가 있을 경우 비용 비교 후 교체
		if (eNavNodeType::Open == corner->eNodeType)
		{
			if (corner->Cost > cost)
			{
				corner->Cost = cost;
				corner->Total = corner->Cost + corner->Dist;
				corner->Parent = node;
			}
		}
		else
		{
			corner->eNodeType = eNavNodeType::Open;
			corner->Parent = node;
			corner->Cost = cost;
			corner->Dist = corner->Center.Distance(end);
			corner->Total = corner->Cost + corner->Dist;

			mVecOpen.push_back(corner);
			mVecUseNode.push_back(corner);
		}
	}
	return false;
}

NavNode* CNavigation::getCorner(eNodeDir dir, NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	switch (dir)
	{
	case eNodeDir::T:
		return getNodeTop(node, endNode, end, outListPath);
	case eNodeDir::RT:
		return getNodeRightTop(node, endNode, end, outListPath);
	case eNodeDir::R:
		return getNodeRight(node, endNode, end, outListPath);
	case eNodeDir::RB:
		return getNodeRightBottom(node, endNode, end, outListPath);
	case eNodeDir::B:
		return getNodeBottom(node, endNode, end, outListPath);
	case eNodeDir::LB:
		return getNodeLeftBottom(node, endNode, end, outListPath);
	case eNodeDir::L:
		return getNodeLeft(node, endNode, end, outListPath);
	case eNodeDir::LT:
		return getNodeLeftTop(node, endNode, end, outListPath);
	default:
		assert(false);
	}
	return nullptr;
}

int CNavigation::sortNode(const void* src, const void* dst)
{
	NavNode* srcNode = (NavNode*)src;
	NavNode* dstNode = (NavNode*)src;

	if (srcNode->Total < dstNode->Total)
	{
		return 1;
	}
	// 내림차순 정렬
	else if (srcNode->Total > dstNode->Total)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

NavNode* CNavigation::getNodeTop(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxY = node->indexY;

	while (true)
	{
		++idxY;

		if (idxY + 1 >= mCountY)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[idxY * mCountX + node->indexX];

		if (checkNode == endNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = node->indexX + 1;
		int cornerY = idxY;

		if (cornerX < mCountX)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY + 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}

		cornerX = node->indexX - 1;
		cornerY = idxY;

		if (cornerX >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY + 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::getNodeRightTop(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxX = node->indexX;
	int idxY = node->indexY;

	while (true)
	{
		++idxX;
		++idxY;

		if (idxX + 1 >= mCountX || idxY + 1 >= mCountY)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[idxY * mCountX + idxX];

		if (endNode == checkNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = idxX - 1;
		int cornerY = idxY;

		if (cornerX >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY + 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}
	
		cornerX = idxX;
		cornerY = idxY - 1;

		if (cornerY >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX + 1)]->eTileType)
			{
				return checkNode;
			}
		}

		NavNode* subSearch = getNodeRight(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}

		subSearch = getNodeTop(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::getNodeRight(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxX = node->indexX;

	while (true)
	{
		++idxX;

		if (idxX + 1 >= mCountX)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[node->indexY * mCountX + idxX];

		if (checkNode == endNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = idxX;
		int cornerY = node->indexY + 1;

		if (cornerY < mCountY)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX + 1)]->eTileType)
			{
				return checkNode;
			}
		}

		cornerX = idxX;
		cornerY = node->indexY - 1;

		if (cornerY >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX + 1)]->eTileType)
			{
				return checkNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::getNodeRightBottom(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxX = node->indexX;
	int idxY = node->indexY;

	while (true)
	{
		++idxX;
		--idxY;

		if (idxX + 1 >= mCountX || idxY - 1 < 0)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[idxY * mCountX + idxX];

		if (endNode == checkNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = idxX;
		int cornerY = idxY + 1;

		if (cornerY < mCountY)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX + 1)]->eTileType)
			{
				return checkNode;
			}
		}
	
		cornerX = idxX - 1;
		cornerY = idxY;

		if (cornerX >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY - 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}

		NavNode* subSearch = getNodeRight(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}

		subSearch = getNodeBottom(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::getNodeBottom(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxY = node->indexY;

	while (true)
	{
		--idxY;

		if (idxY - 1 < 0)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[idxY * mCountX + node->indexX];

		if (checkNode == endNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = node->indexX + 1;
		int cornerY = idxY;

		if (cornerX < mCountX)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY - 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}

		cornerX = node->indexX - 1;
		cornerY = idxY;

		if (cornerX >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY - 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::getNodeLeftBottom(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxX = node->indexX;
	int idxY = node->indexY;

	while (true)
	{
		--idxX;
		--idxY;

		if (idxX - 1 < 0 || idxY - 1 < 0)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[idxY * mCountX + idxX];

		if (endNode == checkNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = idxX;
		int cornerY = idxY + 1;

		if (cornerY < mCountY)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX + 1)]->eTileType)
			{
				return checkNode;
			}
		}
	
		cornerX = idxX + 1;
		cornerY = idxY;

		if (cornerX < mCountX)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY - 1) * mCountX + cornerX]->eTileType)
			{
				return checkNode;
			}
		}

		NavNode* subSearch = getNodeLeft(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}

		subSearch = getNodeBottom(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::getNodeLeft(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxX = node->indexX;

	while (true)
	{
		--idxX;

		if (idxX - 1 < 0)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[node->indexY * mCountX + idxX];

		if (checkNode == endNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = idxX;
		int cornerY = node->indexY + 1;

		if (cornerY < mCountY)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX - 1)]->eTileType)
			{
				return checkNode;
			}
		}

		cornerX = idxX;
		cornerY = node->indexY - 1;

		if (cornerY >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX - 1)]->eTileType)
			{
				return checkNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::getNodeLeftTop(NavNode* node, NavNode* endNode, const Vector3& end, std::list<Vector3>& outListPath)
{
	int idxX = node->indexX;
	int idxY = node->indexY;

	while (true)
	{
		--idxX;
		++idxY;

		if (idxX - 1 < 0 || idxY + 1 >= mCountY)
		{
			return nullptr;
		}

		NavNode* checkNode = mVecNode[idxY * mCountX + idxX];

		if (endNode == checkNode)
		{
			return checkNode;
		}

		if (eNavNodeType::Closed == checkNode->eNodeType)
		{
			return nullptr;
		}

		if (eTileType::Wall == checkNode->eTileType)
		{
			return nullptr;
		}

		int cornerX = idxX + 1;
		int cornerY = idxY;

		if (cornerX < mCountX)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[(cornerY + 1) * mCountX + cornerX ]->eTileType)
			{
				return checkNode;
			}
		}
	
		cornerX = idxX;
		cornerY = idxY - 1;

		if (cornerY >= 0)
		{
			if (eTileType::Wall == mVecNode[cornerY * mCountX + cornerX]->eTileType &&
				eTileType::Normal == mVecNode[cornerY * mCountX + (cornerX - 1)]->eTileType)
			{
				return checkNode;
			}
		}

		NavNode* subSearch = getNodeLeft(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}

		subSearch = getNodeTop(checkNode, endNode, end, outListPath);

		if (subSearch)
		{
			return checkNode;
		}
	}
	return nullptr;
}
