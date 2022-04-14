#pragma once

#include "D2ProcedualMapGenerator.h"
#include "Resource/Mesh/Mesh.h"

struct RoomInfo
{
	class CSpace* Space;
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

class CD2Space
{
	friend class CD2ProcedualMapGenerator;

private:
	CD2Space();
	~CD2Space();

public:
	bool Init();
	void Render();

public:
	// 외부에서 공간 분할 진입하는 함수
	void StartPartitioning(const int sizeX, const int sizeY, const int maxLevel);
	bool MakeRoom();
	void ConnectSpace();

public:
	class CD2Room* GetLeftMostRoomInfo();
	class CD2Room* GetRightMostRoomInfo();
	class CD2Room* GetTopMostRoomInfo();
	class CD2Room* GetBottomMostRoomInfo();
	class CD2Room* GetMostCloseRoomInfo(const int centerIdx);

public:
	bool IsEmpty() const
	{
		return mChild1 == nullptr && mChild2 == nullptr;
	}
	
	CD2Space* Empty()
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

	void SetOwner(CD2ProcedualMapGenerator* com)
	{
		mOwner = com;
	}

	CD2Space* GetNeighbor() const
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

	class CD2ProcedualMapGenerator* GetOwner()
	{
		return mOwner;
	}

private:
	void doPartitioning(const int maxLevel);

private:
	class CD2ProcedualMapGenerator* mOwner;
	CSharedPtr<CMesh> mMesh;
	CSharedPtr<class CShader> mShader;
	class CSpaceConstantBuffer* mCBuffer;
	CD2Space* mParent;
	eSplitDir meSplitDir;
	CD2Space* mChild1;
	CD2Space* mChild2;
	class CD2Room* mRoom;
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
