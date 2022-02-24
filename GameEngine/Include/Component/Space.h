#pragma once

#include "ProcedualMapGenerator.h"

struct RoomInfo
{
	CSpace* Space;
	int RoomID;
	int SizeX;
	int SizeY;
	int LBIdxX;
	int LBIdxY;
	int LBIdx;
	int RTIdxX;
	int RTIdxY;
	int RTIdx;
	int CenterIdxX;
	int CenterIdxY;
	int CenterIdx;

	RoomInfo() :
		Space(nullptr),
		RoomID(0),
		SizeX(0),
		SizeY(0),
		LBIdxX(-1),
		LBIdxY(-1),
		LBIdx(-1),
		RTIdxX(-1),
		RTIdxY(-1),
		RTIdx(-1),
		CenterIdxX(-1),
		CenterIdxY(-1),
		CenterIdx(-1)
	{
	}
};

class CSpace
{
	friend class CProcedualMapGenerator;

private:
	CSpace();
	~CSpace();

public:
	// 외부에서 공간 분할 진입하는 함수
	void StartPartitioning(const int sizeX, const int sizeY, const int maxLevel);
	bool MakeRoom();
	void ConnectSpace();

public:
	RoomInfo* GetLeftMostRoomInfo();
	RoomInfo* GetRightMostRoomInfo();
	RoomInfo* GetTopMostRoomInfo();
	RoomInfo* GetBottomMostRoomInfo();
	RoomInfo* GetMostCloseRoomInfo(const int centerIdx);

public:
	bool IsEmpty() const
	{
		return mChild1 == nullptr && mChild2 == nullptr;
	}
	
	CSpace* Empty()
	{
		if (mChild1)
		{
			mChild1->Empty();
		}
		SAFE_DELETE(mChild1);
		
		if (mChild2)
		{
			mChild2->Empty();
		}
		SAFE_DELETE(mChild2);

		return this;
	}

	void SetOwner(CProcedualMapGenerator* com)
	{
		mOwner = com;
	}

	CSpace* GetNeighbor() const
	{
		if (!mParent)
		{
			return nullptr;
		}

		if (mParent->mChild1 != this)
		{
			return mParent->mChild1;
		}
		return mParent->mChild2;
	}
	
	bool IsConnect() const
	{
		return mbConnect;
	}

private:
	void doPartitioning(const int maxLevel);

private:
	class CProcedualMapGenerator* mOwner;
	CSpace* mParent;
	eSplitDir meSplitDir;
	CSpace* mChild1;
	CSpace* mChild2;
	RoomInfo* mRoomInfo;
	int mSpaceID;
	int mSizeX;
	int mSizeY;
	int mLBIdxX;
	int mLBIdxY;
	int mLBIdx;
	int mRTIdxX;
	int mRTIdxY;
	int mRTIdx;
	int mCenterIdxX;
	int mCenterIdxY;
	int mCenterIdx;
	int mLevel;
	bool mbConnect;
};

