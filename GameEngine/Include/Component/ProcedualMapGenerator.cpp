#include <time.h>
#include "ProcedualMapGenerator.h"
#include "Space.h"
#include "../GameObject/GameObject.h"
#include "../Engine.h"

CProcedualMapGenerator::CProcedualMapGenerator()	:
	mMapCountX(0),
	mMapCountY(0),
	mPartitionLevel(0),
	mRoomSizeMin(5),
	meTileShape(eTileShape::Rhombus),
	mTileSize(160.f, 80.f, 0.f),
	mSpaceTree(nullptr),
	mTileDiagonal(0.f)
{
	mbIsRender = true;

	SetTypeID<CProcedualMapGenerator>();
	
	for (int i = 0; i < (int)eGenerationStep::Max; ++i)
	{
		mGenerationStep[i] = false;
	}

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

	for (int i = 0; i < (int)eGenerationStep::Max; ++i)
	{
		mGenerationStep[i] = com.mGenerationStep[i];
	}
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
	FindTileComponent();
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

	if (ConnectRoom())
	{
		return false;
	}

	return true;
}

void CProcedualMapGenerator::FindTileComponent()
{
	mTileMap = mObject->FindSceneComponentFromType<CTileMapComponent>();
}

bool CProcedualMapGenerator::DoStep(eGenerationStep step)
{
	if (mMapCountX == 0 || mMapCountY == 0 || mPartitionLevel == 1)
	{
		return false;
	}

	switch (step)
	{
	case eGenerationStep::PartitionSpace:
		return PartitionSpace();
	case eGenerationStep::MakeRoom:
		return MakeRoom();
	case eGenerationStep::ConnectRoom:
		return ConnectRoom();
		break;
	default:
		assert(false);
		return false;
	}
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

	mGenerationStep[(int)eGenerationStep::PartitionSpace] = true;

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

	mGenerationStep[(int)eGenerationStep::MakeRoom] = true;
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

	mGenerationStep[(int)eGenerationStep::ConnectRoom] = true;
	return true;
}

bool CProcedualMapGenerator::PassToTileMap()
{
	return true;
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

eGenerationStep CProcedualMapGenerator::GetCurrentGenerationStepToDo() const
{
	for (int i = 0; i < (int)eGenerationStep::Max; ++i)
	{
		if (!mGenerationStep[i])
		{
			return (eGenerationStep)i;
		}
	}
}

int CProcedualMapGenerator::GetCurrentGenerationStepDone() const
{
	eGenerationStep step = GetCurrentGenerationStepToDo();
	if (eGenerationStep::PartitionSpace == step)
	{
		return -1;
	}
	return (int)step - 1;
}

void CProcedualMapGenerator::resetMapInfo()
{
	size_t size = mVecMapInfo.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecMapInfo[i] = eTileType::Wall;
	}
}
