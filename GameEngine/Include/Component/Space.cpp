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
	
	// ���ҽ���
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
	// �� ������ �� �̻� ���ҵ��� ���� �����̸� �� �����Ѵ�.
	else
	{
		mRoomInfo = new RoomInfo;
		mRoomInfo->Space = this;

		// �� ������ ���� ���� ��迡�� 2ĭ�� ������ ��
		int minIdxX = mLBIdxX + 2;
		int minIdxY = mLBIdxY + 2;
		int maxIdxX = mRTIdxX - 2;
		int maxIdxY = mRTIdxY - 2;

		int minSizeX = mOwner->GetRoomSizeMin();
		int maxSizeX = maxIdxX - minIdxX + 1;
		int minSizeY = mOwner->GetRoomSizeMin();
		int maxSizeY = maxIdxY - minIdxY + 1;

		// �ִ� ũ�⿡�� 75~100% ũ����� ���� ������� ������ ����
		mRoomInfo->SizeX = (75 + (rand() % 26) / 100.f) * maxSizeX;
		mRoomInfo->SizeY = (75 + (rand() % 26) / 100.f) * maxSizeY;

		// �ּ� ũ�⺸�� �۰� ���´ٸ�, �ּ� ũ��� ����
		if (mRoomInfo->SizeX < minSizeX)
		{
			mRoomInfo->SizeX = minSizeX;
		}
		if (mRoomInfo->SizeY < minSizeY)
		{
			mRoomInfo->SizeY = minSizeY;
		}

		// ���� �Ʒ� �ε��� ����
		mRoomInfo->LBIdxX = minIdxX + (rand() % (maxIdxX - minIdxX + 1 - mRoomInfo->SizeX));
		mRoomInfo->LBIdxY = minIdxY + (rand() % (maxIdxY - minIdxY + 1 - mRoomInfo->SizeY));
		mRoomInfo->LBIdx = mRoomInfo->LBIdxY * mOwner->GetMapCountX() + mRoomInfo->LBIdxX;
		
		// ������ �ε���
		mRoomInfo->RTIdxX = mRoomInfo->LBIdxX + (mRoomInfo->SizeX - 1);
		mRoomInfo->RTIdxY = mRoomInfo->LBIdxY + (mRoomInfo->SizeY - 1);
		mRoomInfo->RTIdx = mRoomInfo->RTIdxY * mOwner->GetMapCountX() + mRoomInfo->RTIdxX;

		// �߾� �ε���
		mRoomInfo->CenterIdxX = mRoomInfo->LBIdxX + (mRoomInfo->SizeX / 2);
		mRoomInfo->CenterIdxY = mRoomInfo->LBIdxY + (mRoomInfo->SizeY / 2);
		mRoomInfo->CenterIdx = mRoomInfo->CenterIdxY * mOwner->GetMapCountX() + mRoomInfo->CenterIdxX;

		mOwner->AddRoom(mRoomInfo);
	}
	return true;
}

void CSpace::ConnectSpace()
{
	// �ֻ��� ������ ���
	if (!mParent)
	{
		return;
	}

	CSpace* neighbor = GetNeighbor();
	mbConnect = true;
	neighbor->mbConnect = true;

	if (eSplitDir::Horizontal == meSplitDir)
	{
		// �� ������ ���� ������ ���
		if (mCenterIdxX < neighbor->mCenterIdxX)
		{
			RoomInfo* right = GetRightMostRoomInfo();


		}
		// �������� ���
		else
		{

		}
	}
	else
	{
		// �� ������ �Ʒ��� ������ ���
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

	// �ִ� ���� ������ ������ ��� ���� �ߴ�
	if (maxLevel == mLevel)
	{
		return;
	}

	// ���� ������ �ּ� ���� ������ ( �� ������ + 2) ���� ���� ��� ���� �ߴ�
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

	// 4 : 6 ~ 6 : 4�� ������ ũ�� �й�
	float ratio = (4.f + (float)(rand() % 3)) / 10.f;
	
	// ����
	if (eSplitDir::Horizontal == dir)
	{
		// child1 �� �Ʒ��� 
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
		// child1�� ����
		mChild1->mLBIdxX = mLBIdxX;
		mChild1->mLBIdxY = mLBIdxY;

		mChild1->mRTIdxX = mLBIdxX + (mSizeX * ratio);
		mChild1->mRTIdxY = mRTIdxY;

		mChild2->mLBIdxX = mLBIdxX + (mSizeX * ratio) + 1;
		mChild2->mLBIdxY = mLBIdxY;

		mChild2->mRTIdxX = mRTIdxX;
		mChild2->mRTIdxY = mRTIdxY;
	}

	// ������ ���
	mChild1->mSizeX = mChild1->mRTIdxX - mChild1->mLBIdxX + 1;
	mChild1->mSizeY = mChild1->mRTIdxY - mChild1->mLBIdxY + 1;

	mChild2->mSizeX = mChild2->mRTIdxX - mChild2->mLBIdxX + 1;
	mChild2->mSizeY = mChild2->mRTIdxY - mChild2->mLBIdxY + 1;

	// ������ �ε��� ���
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

	// �ڽ� ���� ����
	mChild1->doPartitioning(maxLevel);
	mChild2->doPartitioning(maxLevel);
}
