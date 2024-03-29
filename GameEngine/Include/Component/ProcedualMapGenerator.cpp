#include <time.h>
#include "ProcedualMapGenerator.h"
#include "Space.h"
#include "../GameObject/GameObject.h"
#include "../Engine.h"
#include "../Resource/TileSet/TileSet.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Collision/CollisionManager.h"

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
	if (mSpaceTree)
	{
		mSpaceTree->Empty();
		SAFE_DELETE(mSpaceTree);
	}
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

void CProcedualMapGenerator::Save(FILE* fp)
{
	fwrite(&meTileShape, sizeof(eTileShape), 1, fp);
	fwrite(&mTileSize, sizeof(Vector3), 1, fp);
	fwrite(&mTileDiagonal, sizeof(float), 1, fp);
	fwrite(&mMatIsoToWorld, sizeof(Matrix), 1, fp);
	fwrite(&mRoomSizeMin, sizeof(int), 1, fp);
	fwrite(&mMapCountX, sizeof(int), 1, fp);
	fwrite(&mMapCountY, sizeof(int), 1, fp);
	fwrite(&mPartitionLevel, sizeof(int), 1, fp);

	bool tileSetExist = false;
	if (mTileSet)
	{
		tileSetExist = true;
	}
	fwrite(&tileSetExist, sizeof(bool), 1, fp);

	if (tileSetExist)
	{
		std::string tileSetFullPath = mTileSet->GetCSVFullPath();
		int length = (int)tileSetFullPath.length();
		fwrite(&length, sizeof(int), 1, fp);
		fwrite(tileSetFullPath.c_str(), sizeof(char), length, fp);
	}

	bool wallTileSetExist = false;
	if (mWallTileSet)
	{
		wallTileSetExist = true;
	}
	fwrite(&wallTileSetExist, sizeof(bool), 1, fp);

	if (wallTileSetExist)
	{
		std::string wallTileSetFullPath = mWallTileSet->GetCSVFullPath();
		int length = (int)wallTileSetFullPath.length();
		fwrite(&length, sizeof(int), 1, fp);
		fwrite(wallTileSetFullPath.c_str(), sizeof(char), length, fp);
	}

	// 타일맵은 자동 생성할 것이기 떄문에 삭제한다. 
	// 부모 Save 먼저 호출되면 안 됨!! ( child 우선으로 저장하기 떄문에 )
	if (mTileMap)
	{
		mTileMap->DeleteChild(mTileMap->GetWallComponent());
		mObject->DeleteComponent(mTileMap);
	}

	CSceneComponent::Save(fp);
}

void CProcedualMapGenerator::Load(FILE* fp)
{
	fread(&meTileShape, sizeof(eTileShape), 1, fp);
	fread(&mTileSize, sizeof(Vector3), 1, fp);
	fread(&mTileDiagonal, sizeof(float), 1, fp);
	fread(&mMatIsoToWorld, sizeof(Matrix), 1, fp);
	fread(&mRoomSizeMin, sizeof(int), 1, fp);
	fread(&mMapCountX, sizeof(int), 1, fp);
	fread(&mMapCountY, sizeof(int), 1, fp);
	fread(&mPartitionLevel, sizeof(int), 1, fp);

	bool tileSetExist = false;
	fread(&tileSetExist, sizeof(bool), 1, fp);

	if (tileSetExist)
	{
		int length = 0;
		fread(&length, sizeof(int), 1, fp);
		char tileSetFullPath[MAX_PATH] = {};
		fread(tileSetFullPath, sizeof(char), length, fp);
		std::string tileSetName;
		mObject->GetScene()->GetResource()->LoadTileSetFullPath(tileSetName, tileSetFullPath);
		mTileSet = mObject->GetScene()->GetResource()->FindTileSet(tileSetName);
	}

	bool wallTileSetExist = false;
	fread(&wallTileSetExist, sizeof(bool), 1, fp);
	if (wallTileSetExist)
	{
		int length = 0;
		fread(&length, sizeof(int), 1, fp);
		char wallTileSetFullPath[MAX_PATH] = {};
		fread(wallTileSetFullPath, sizeof(char), length, fp);
		std::string wallTileSetName;
		mObject->GetScene()->GetResource()->LoadTileSetFullPath(wallTileSetName, wallTileSetFullPath);
		mWallTileSet = mObject->GetScene()->GetResource()->FindTileSet(wallTileSetName);
	}

	CSceneComponent::Load(fp);
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

	if (!GenerateTile())
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
	
	// 모두 Wall 로 초기화
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

	// 만들어진 방 인덱스들 기반으로 데이터 쓰기
	size_t size = mVecRoomInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		for (size_t y = mVecRoomInfo[i]->LBIdxY; y <= mVecRoomInfo[i]->RTIdxY; ++y)
		{
			for (size_t x = mVecRoomInfo[i]->LBIdxX; x <= mVecRoomInfo[i]->RTIdxX; ++x)
			{
				int idx = y * mMapCountX + x;

				// 아래 코너
				if (mVecRoomInfo[i]->LBIdxX == x && mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerS;
					continue;
				}
				// 위 코너
				if (mVecRoomInfo[i]->RTIdxX == x && mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerN;
					continue;
				}
				// 왼쪽 코너
				if (mVecRoomInfo[i]->LBIdxX == x && mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerW;
					continue;
				}
				// 오른쪽 코너
				if (mVecRoomInfo[i]->RTIdxX == x && mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::CornerE;
					continue;
				}

				// 오른쪽 아래 벽
				if (mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::WallSE;
					continue;
				}
				// 왼쪽 위 벽
				if (mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eTileType::WallNW;
					continue;
				}
				// 왼쪽 아래 벽
				if (mVecRoomInfo[i]->LBIdxX == x)
				{
					mVecMapInfo[idx] = eTileType::WallSW;
					continue;
				}
				// 오른쪽 위 벽
				if (mVecRoomInfo[i]->RTIdxX == x)
				{
					mVecMapInfo[idx] = eTileType::WallNE;
					continue;
				}

				// 나머지 경우는 갈 수 있는 타일로 처리한다.
				mVecMapInfo[idx] = eTileType::Normal;
			}
		}
	}

 //	// 실제 타일맵 생성
 //	if (!GenerateTile())
 //	{
 //		return false;
 //	}
 //
	return true;
}

bool CProcedualMapGenerator::ConnectRoom()
{
	if (mVecSpaceByLevel.empty())
	{
		return false;
	}

	// 깊은 레벨 방들부터 이어야 하므로 뒤에서부터 순회한다.
	auto iter = mVecSpaceByLevel.rbegin();
	auto iterEnd = mVecSpaceByLevel.rend();
	for (; iter != iterEnd; ++iter)
	{
		size_t size = iter->size();
		for (size_t i = 0; i < size; ++i)
		{
			// 이미 연결된 방이라면 건너뛴다.
			if ((*iter)[i]->IsConnect())
			{
				continue;
			}
			
			(*iter)[i]->ConnectSpace();
		}
	}

 //	if (!GenerateTile())
 //	{
 //		return false;
 //	}

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
