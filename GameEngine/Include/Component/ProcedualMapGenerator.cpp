#include <time.h>
#include "ProcedualMapGenerator.h"

CProcedualMapGenerator::CProcedualMapGenerator()	:
	mMapCountX(0),
	mMapCountY(0),
	mPartitionLevel(0),
	mRoomSizeMin(5)
{
	SetTypeID<CProcedualMapGenerator>();
}

CProcedualMapGenerator::CProcedualMapGenerator(const CProcedualMapGenerator& com)	:
	CObjectComponent(com)
{
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
	CObjectComponent::Init();
	return true;
}

void CProcedualMapGenerator::Start()
{
	CObjectComponent::Start();
}

void CProcedualMapGenerator::Update(float deltaTime)
{
	CObjectComponent::Update(deltaTime);
}

void CProcedualMapGenerator::PostUpdate(float deltaTime)
{
	CObjectComponent::PostUpdate(deltaTime);
}

void CProcedualMapGenerator::PrevRender()
{
	CObjectComponent::PrevRender();
}

void CProcedualMapGenerator::Render()
{
	CObjectComponent::Render();
}

void CProcedualMapGenerator::PostRender()
{
	CObjectComponent::PostRender();
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

	if (ConnectRoom())
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
	mVecSpaceByLevel.resize(mPartitionLevel);
	
	mSpaceTree = new CSpace;
	mSpaceTree->SetOwner(this);
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
					mVecMapInfo[idx] = eMapObject::TileCornerBottom;
					continue;
				}
				// �� �ڳ�
				if (mVecRoomInfo[i]->RTIdxX == x && mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eMapObject::TileCornerTop;
					continue;
				}
				// ���� �ڳ�
				if (mVecRoomInfo[i]->LBIdxX == x && mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eMapObject::TileCornerLeft;
					continue;
				}
				// ������ �ڳ�
				if (mVecRoomInfo[i]->RTIdxX == x && mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eMapObject::TileCornerRight;
					continue;
				}

				// ������ �Ʒ� ��
				if (mVecRoomInfo[i]->LBIdxY == y)
				{
					mVecMapInfo[idx] = eMapObject::TileWallBottomRight;
					continue;
				}
				// ���� �� ��
				if (mVecRoomInfo[i]->RTIdxY == y)
				{
					mVecMapInfo[idx] = eMapObject::TileWallLeftTop;
					continue;
				}
				// ���� �Ʒ� ��
				if (mVecRoomInfo[i]->LBIdxX == x)
				{
					mVecMapInfo[idx] = eMapObject::TileWallLeftBottom;
					continue;
				}
				// ������ �� ��
				if (mVecRoomInfo[i]->RTIdxX == x)
				{
					mVecMapInfo[idx] = eMapObject::TileWallTopRight;
					continue;
				}

				// ������ ���� �� �� �ִ� Ÿ�Ϸ� ó���Ѵ�.
				mVecMapInfo[idx] = eMapObject::TileNormal;
			}
		}
	}
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
}

bool CProcedualMapGenerator::PassToTileMap()
{
	return true;
}
