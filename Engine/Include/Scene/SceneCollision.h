#pragma once

#include "../GameInfo.h"

struct CollisionSectionInfo
{
	std::vector<class CCollisionSection*> vecSection;
	Vector3 SectionSize;		// 분할된 공간 한 칸의 크기
	Vector3 Center;				// 전체 영역 중심
	Vector3 SectionTotalSize;	// 전체 영역 크기
	Vector3 Min;				// 전체 영역에서의 최소값
	Vector3 Max;				// 전체 영역에서의 최대값
	int CountX;					// 몇 개로 분할되어 있는지
	int CountY;
	int CountZ;

	CollisionSectionInfo():
		CountX(1),
		CountY(1),
		CountZ(1)
	{
	}
};

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

public:
	bool Init();
	void Start();

private:
	class CScene* mScene;
	CollisionSectionInfo* mSectionInfo;
};

