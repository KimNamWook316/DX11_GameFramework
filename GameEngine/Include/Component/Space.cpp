#include "Space.h"

CSpace::CSpace()	:
	mOwner(nullptr),
	mParent(nullptr),
	mChild1(nullptr),
	mChild2(nullptr),
	meSplitDir(eSplitDir::None),
	mRoomInfo(nullptr),
	mSizeX(0),
	mSizeY(0),
	mSpaceID(-1),
	mLBIdxX(-1),
	mLBIdxY(-1),
	mLBIdx(-1),
	mRTIdxX(-1),
	mRTIdxY(-1),
	mRTIdx(-1),
	mCenterIdxX(-1),
	mCenterIdxY(-1),
	mCenterIdx(-1),
	mLevel(0),
	mbConnect(false)
{
}

CSpace::~CSpace()
{
	SAFE_DELETE(mRoomInfo);
}

void CSpace::StartPartitioning(const int sizeX, const int sizeY, const int maxLevel)
{
	if (!mParent)
	{
		mLBIdxX = 0;
		mLBIdxY = 0;
		mLBIdx = 0;

		mRTIdxX = sizeX - 1;
		mRTIdxY = sizeY - 1;
		mRTIdx = sizeX * sizeY - 1;

		mCenterIdxX = sizeX / 2;
		mCenterIdxY = sizeY / 2;
		mCenterIdx = mCenterIdxY * mOwner->GetMapCountX() + mCenterIdxX;

		mSizeX = sizeX;
		mSizeY = sizeY;
	}
	
	// 분할시작
	doPartitioning(maxLevel);
}

bool CSpace::MakeRoom()
{
	if (mChild1)
	{
		return mChild1->MakeRoom();
	}
	if (mChild2)
	{
		return mChild2->MakeRoom();
	}
	// 이 공간이 더 이상 분할되지 않은 공간이면 방 생성한다.
	else
	{
		mRoomInfo = new RoomInfo;
		mRoomInfo->Space = this;

		// 길 생성을 위해 공간 경계에서 2칸은 띄워줘야 함
		int minIdxX = mLBIdxX + 2;
		int minIdxY = mLBIdxY + 2;
		int maxIdxX = mRTIdxX - 2;
		int maxIdxY = mRTIdxY - 2;

		int minSizeX = mOwner->GetRoomSizeMin();
		int maxSizeX = maxIdxX - minIdxX + 1;
		int minSizeY = mOwner->GetRoomSizeMin();
		int maxSizeY = maxIdxY - minIdxY + 1;

		// 최대 크기에서 75~100% 크기까지 랜덤 사이즈로 사이즈 설정
		mRoomInfo->SizeX = (75 + (rand() % 26) / 100.f) * maxSizeX;
		mRoomInfo->SizeY = (75 + (rand() % 26) / 100.f) * maxSizeY;

		// 최소 크기보다 작게 나온다면, 최소 크기로 설정
		if (mRoomInfo->SizeX < minSizeX)
		{
			mRoomInfo->SizeX = minSizeX;
		}
		if (mRoomInfo->SizeY < minSizeY)
		{
			mRoomInfo->SizeY = minSizeY;
		}

		// 왼쪽 아래 인덱스 설정
		mRoomInfo->LBIdxX = minIdxX + (rand() % (maxIdxX - minIdxX + 1 - mRoomInfo->SizeX));
		mRoomInfo->LBIdxY = minIdxY + (rand() % (maxIdxY - minIdxY + 1 - mRoomInfo->SizeY));
		mRoomInfo->LBIdx = mRoomInfo->LBIdxY * mOwner->GetMapCountX() + mRoomInfo->LBIdxX;
		
		// 오른쪽 인덱스
		mRoomInfo->RTIdxX = mRoomInfo->LBIdxX + (mRoomInfo->SizeX - 1);
		mRoomInfo->RTIdxY = mRoomInfo->LBIdxY + (mRoomInfo->SizeY - 1);
		mRoomInfo->RTIdx = mRoomInfo->RTIdxY * mOwner->GetMapCountX() + mRoomInfo->RTIdxX;

		// 중앙 인덱스
		mRoomInfo->CenterIdxX = mRoomInfo->LBIdxX + (mRoomInfo->SizeX / 2);
		mRoomInfo->CenterIdxY = mRoomInfo->LBIdxY + (mRoomInfo->SizeY / 2);
		mRoomInfo->CenterIdx = mRoomInfo->CenterIdxY * mOwner->GetMapCountX() + mRoomInfo->CenterIdxX;

		mOwner->AddRoom(mRoomInfo);
	}
	return true;
}

void CSpace::ConnectSpace()
{
	// 최상위 공간인 경우
	if (!mParent)
	{
		return;
	}

	CSpace* neighbor = GetNeighbor();
	mbConnect = true;
	neighbor->mbConnect = true;

	if (eSplitDir::Horizontal == meSplitDir)
	{
		// 이 공간이 왼쪽 공간인 경우
		if (mCenterIdxX < neighbor->mCenterIdxX)
		{
			RoomInfo* right = GetRightMostRoomInfo();


		}
		// 오른쪽인 경우
		else
		{

		}
	}
	else
	{
		// 이 공간이 아래쪽 공간인 경우
		if (mCenterIdxY < neighbor->mCenterIdxY)
		{

		}
		else
		{

		}
	}
}

RoomInfo* CSpace::GetLeftMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->LBIdxX < room2->LBIdxX)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetRightMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->RTIdxX > room2->RTIdxX)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetTopMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->RTIdxY > room2->RTIdxY)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetBottomMostRoomInfo()
{
	if (mRoomInfo)
	{
		return mRoomInfo;
	}

	RoomInfo* room1 = mChild1->GetLeftMostRoomInfo();
	RoomInfo* room2 = mChild2->GetLeftMostRoomInfo();

	if (room1->LBIdxY < room2->LBIdxY)
	{
		return room1;
	}

	return room2;
}

RoomInfo* CSpace::GetMostCloseRoomInfo(const int centerIdx)
{
	return nullptr;
}

void CSpace::doPartitioning(const int maxLevel)
{	
	if (mParent)
	{
		mLevel = mParent->mLevel + 1;
	}

	mOwner->AddSpace(this, mLevel);

	// 최대 분할 레벨에 도달한 경우 분할 중단
	if (maxLevel == mLevel)
	{
		return;
	}

	// 생성 가능한 최소 공간 사이즈 ( 방 사이즈 + 2) 보다 작은 경우 분할 중단
	if (mSizeX < mOwner->GetRoomSizeMin() + 2 || mSizeY < mOwner->GetRoomSizeMin() + 2)
	{
		return;
	}

	mChild1 = new CSpace;
	mChild2 = new CSpace;

	mChild1->mParent = this;
	mChild2->mParent = this;

	mChild1->mOwner = mOwner;
	mChild2->mOwner = mOwner;

	eSplitDir dir;

	if (mSizeX <= mSizeY)
	{
		dir = eSplitDir::Horizontal;
	}
	else
	{
		dir = eSplitDir::Vertical;
	}

	mChild1->meSplitDir = dir;
	mChild2->meSplitDir = dir;

	// 4 : 6 ~ 6 : 4의 비율로 크기 분배
	float ratio = (4.f + (float)(rand() % 3)) / 10.f;
	
	// 분할
	if (eSplitDir::Horizontal == dir)
	{
		// child1 이 아래쪽 
		mChild1->mLBIdxX = mLBIdxX;
		mChild1->mLBIdxY = mLBIdxY;

		mChild1->mRTIdxX = mRTIdxX;
		mChild1->mRTIdxY = mLBIdxY + (mSizeY * ratio);

		mChild2->mLBIdxX = mLBIdxX;
		mChild2->mLBIdxY = mLBIdxY + (mSizeY * ratio) + 1;

		mChild2->mRTIdxX = mRTIdxX;
		mChild2->mRTIdxY = mRTIdxY;
	}
	else
	{
		// child1이 왼쪽
		mChild1->mLBIdxX = mLBIdxX;
		mChild1->mLBIdxY = mLBIdxY;

		mChild1->mRTIdxX = mLBIdxX + (mSizeX * ratio);
		mChild1->mRTIdxY = mRTIdxY;

		mChild2->mLBIdxX = mLBIdxX + (mSizeX * ratio) + 1;
		mChild2->mLBIdxY = mLBIdxY;

		mChild2->mRTIdxX = mRTIdxX;
		mChild2->mRTIdxY = mRTIdxY;
	}

	// 사이즈 계산
	mChild1->mSizeX = mChild1->mRTIdxX - mChild1->mLBIdxX + 1;
	mChild1->mSizeY = mChild1->mRTIdxY - mChild1->mLBIdxY + 1;

	mChild2->mSizeX = mChild2->mRTIdxX - mChild2->mLBIdxX + 1;
	mChild2->mSizeY = mChild2->mRTIdxY - mChild2->mLBIdxY + 1;

	// 나머지 인덱스 계산
	mChild1->mLBIdx = mChild1->mLBIdxY * mOwner->GetMapCountX() + mChild1->mLBIdxX;
	mChild1->mRTIdx = mChild1->mRTIdxY * mOwner->GetMapCountX() + mChild1->mRTIdxX;
	mChild1->mCenterIdxX = mChild1->mLBIdxX + (mChild1->mSizeX / 2);
	mChild1->mCenterIdxY = mChild1->mLBIdxY + (mChild1->mSizeY / 2);
	mChild1->mCenterIdx = mChild1->mCenterIdxY * mOwner->GetMapCountX() + mChild1->mCenterIdxX;

	mChild2->mLBIdx = mChild2->mLBIdxY * mOwner->GetMapCountX() + mChild2->mLBIdxX;
	mChild2->mRTIdx = mChild2->mRTIdxY * mOwner->GetMapCountX() + mChild2->mRTIdxX;
	mChild2->mCenterIdxX = mChild2->mLBIdxX + (mChild2->mSizeX / 2);
	mChild2->mCenterIdxY = mChild2->mLBIdxY + (mChild2->mSizeY / 2);
	mChild2->mCenterIdx = mChild2->mCenterIdxY * mOwner->GetMapCountX() + mChild2->mCenterIdxX;

	// 자식 공간 분할
	mChild1->doPartitioning(maxLevel);
	mChild2->doPartitioning(maxLevel);
}
