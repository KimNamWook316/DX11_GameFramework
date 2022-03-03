#include <time.h>
#include "ProcedualMapGenerator.h"
#include "Space.h"
#include "../GameObject/GameObject.h"
#include "../Engine.h"
#include "../Resource/TileSet/TileSet.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CProcedualMapGenerator::CProcedualMapGenerator()	:
	mMapCountX(0),
	mMapCountY(0),
	mPartitionLevel(0),
	mRoomSizeMin(6),
	meTileShape(eTileShape::Rhombus),
	mTileSize(160.f, 80.f, 0.f),
	mSpaceTree(nullptr),
	mTileDiagonal(0.f)
{
	mbIsRender = true;

	SetTypeID<CProcedualMapGenerator>();

	mMatIsoToWorld.v[0] = Vector4((2.f * sqrt(5.f)/ 5.f) - 0.01f , (sqrt(5.f) / 5.f) - 0.01f, 0.f, 0.f);
	mMatIsoToWorld.v[1] = Vector4((-2.f * sqrt(5.f) / 5.f) + 0.01f, (sqrt(5.f) / 5.f) - 0.01f, 0.f, 0.f);
	mMatIsoToWorld.v[2] = Vector4(0.f, 0.f, 1.f, 0.f);
	mMatIsoToWorld.v[3] = Vector4(0.f, 0.f, 0.f, 1.f);

	mTileDiagonal = sqrt(((mTileSize.x / 2.f) * (mTileSize.x / 2.f)) + ((mTileSize.y / 2.f) * (mTileSize.y / 2.f)));
}

CProcedualMapGenerator::CProcedualMapGenerator(const CProcedualMapGenerator& com)	:
	CSceneComponent(com)
{
	*this = com;
	mSpaceTree = nullptr;
}

CProcedualMapGenerator::~CProcedualMapGenerator()
{
	mSpaceTree->Empty();
	SAFE_DELETE(mSpaceTree);
	mVecMapInfo.clear();
}

bool CProcedualMapGenerator::Init()
{
	CSceneComponent::Init();
	return true;
}

void CProcedualMapGenerator::Start()
{
	CSceneComponent::Start();
	CreateTileComponent();
}

void CProcedualMapGenerator::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CProcedualMapGenerator::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CProcedualMapGenerator::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CProcedualMapGenerator::Render()
{
	CSceneComponent::Render();

	if (CEngine::GetInst()->IsDebugMode() && mSpaceTree)
	{
		mSpaceTree->Render();
	}
}

void CProcedualMapGenerator::PostRender()
{
	CSceneComponent::PostRender();
}

CProcedualMapGenerator* CProcedualMapGenerator::Clone()
{
	return new CProcedualMapGenerator(*this);
}

bool CProcedualMapGenerator::GenerateMap()
{
	if (mMapCountX == 0 || mMapCountY == 0 || mPartitionLevel == 1)
	{
		return false;
	}

	if (!PartitionSpace())
	{
		return false;
	}
	
	if (!MakeRoom())
	{
		return false;
	}

	if (!ConnectRoom())
	{
		return false;
	}

	if (!GenerateWall())
	{
		return false;
	}

	mTileMap->SetNavigationData();

	return true;
}

bool CProcedualMapGenerator::CreateTileComponent()
{
	mTileMap = mObject->FindSceneComponentFromType<CTileMapComponent>();

	if (mTileMap)
	{
		return true;
	}

	if (!mTileMap)
	{
		mTileMap = mObject->CreateComponent<CTileMapComponent>("procedualTileMap");
	}

	if (!mTileMap)
	{
		return false;
	}

	AddChild(mTileMap);

	return true;
}

bool CProcedualMapGenerator::CreateWallComponent()
{
	if (!mTileMap->CreateWallComponent())
	{
		return false;
	}
	return true;
}

bool CProcedualMapGenerator::PartitionSpace()
{
	if (mMapCountX == 0 || mMapCountY == 0 || mPartitionLevel == 0)
	{
		return false;
	}

	srand(unsigned int(time(NULL)));

	mVecMapInfo.resize(mMapCountX * mMapCountY);
	
	// ��� Wall �� �ʱ�ȭ
	resetMapInfo();

	mVecSpaceByLevel.resize(mPartitionLevel + 1);
	
	mSpaceTree = new CSpace;
	mSpaceTree->SetOwner(this);
	mSpaceTree->Init();

	mSpaceTree->StartPartitioning(mMapCountX, mMapCountY, mPartitionLevel);

	return true;
}

bool CProcedualMapGenerator::MakeRoom()
{
	if (!mSpaceTree)
	{
		return false;
	}

	if (!mSpaceTree->MakeRoom())
	{
		return false;
	}

	// ������� �� �ε����� ������� ������ ����
	size_t size = mVecRoomInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		for (size_t y = mVecRoomInfo[i]->LBIdxY; y <= mVecRoomInfo[i]->RTIdxY; ++y)
		{
			for (size_t x = mVecRoomInfo[i]->LBIdxX; x <= mVecRoomInfo[i]->RTIdxX; ++x)
			{
				int idx = y * mMapCountX + x;

				// �Ʒ� �ڳ�
				if (mVecRoomInfo[i]->LBIdxX == x && mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerS;
					continue;
				}
				// �� �ڳ�
				if (mVecRoomInfo[i]->RTIdxX == x && mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerN;
					continue;
				}
				// ���� �ڳ�
				if (mVecRoomInfo[i]->LBIdxX == x && mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerW;
					continue;
				}
				// ������ �ڳ�
				if (mVecRoomInfo[i]->RTIdxX == x && mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerE;
					continue;
				}

				// ������ �Ʒ� ��
				if (mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::WallSE;
					continue;
				}
				// ���� �� ��
				if (mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::WallNW;
					continue;
				}
				// ���� �Ʒ� ��
				if (mVecRoomInfo[i]->LBIdxX == x)
				{
					mVecMapInfo[idx] = eTileType::WallSW;
					continue;
				}
				// ������ �� ��
				if (mVecRoomInfo[i]->RTIdxX == x)
				{
					mVecMapInfo[idx] = eTileType::WallNE;
					continue;
				}

				// ������ ���� �� �� �ִ� Ÿ�Ϸ� ó���Ѵ�.
				mVecMapInfo[idx] = eTileType::Normal;
			}
		}
	}

	// ���� Ÿ�ϸ� ����
	if (!GenerateTile())
	{
		return false;
	}

	return true;
}

bool CProcedualMapGenerator::ConnectRoom()
{
	if (mVecSpaceByLevel.empty())
	{
		return false;
	}

	// ���� ���� ������ �̾�� �ϹǷ� �ڿ������� ��ȸ�Ѵ�.
	auto iter = mVecSpaceByLevel.rbegin();
	auto iterEnd = mVecSpaceByLevel.rend();
	for (; iter != iterEnd; ++iter)
	{
		size_t size = iter->size();
		for (size_t i = 0; i < size; ++i)
		{
			// �̹� ����� ���̶�� �ǳʶڴ�.
			if ((*iter)[i]->IsConnect())
			{
				continue;
			}
			
			(*iter)[i]->ConnectSpace();
		}
	}

	if (!GenerateTile())
	{
		return false;
	}

	return true;
}

bool CProcedualMapGenerator::GenerateTile()
{
	if (!mTileMap)
	{
		if (!CreateTileComponent())
		{
			return false;
		}
	}

	mTileMap->SetTileSet(mTileSet);

	ProcedualMapData data;
	data.TileCountX = mMapCountX;
	data.TileCountY = mMapCountY;
	data.TileInfo = mVecMapInfo;
	data.TileShape = meTileShape;
	data.TileSize = mTileSize;

	if (!mTileMap->CreateTileProcedual(data))
	{
		return false;
	}

	return true;
}

bool CProcedualMapGenerator::GenerateWall()
{
	if (!mTileMap->CreateWallComponent())
	{
		return false;
	}

	mTileMap->SetWallTileSet(mWallTileSet);
	return mTileMap->CreateWall();
}

void CProcedualMapGenerator::SetTileType(const int idx, eTileType eMapObj)
{
	if (idx < 0 || idx >= mVecMapInfo.size())
	{
		return;
	}
	
	mVecMapInfo[idx] = eMapObj;
}

void CProcedualMapGenerator::SetTileType(const int x, const int y, eTileType eMapObj)
{
	SetTileType(y * mMapCountX + x, eMapObj);
}

void CProcedualMapGenerator::resetMapInfo()
{
	size_t size = mVecMapInfo.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecMapInfo[i] = eTileType::Wall;
	}
}
