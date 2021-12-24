#pragma once

#include "../GameInfo.h"

struct CollisionSectionInfo
{
	std::vector<class CCollisionSection*> vecSection;
	Vector3 SectionSize;		// ���ҵ� ���� �� ĭ�� ũ��
	Vector3 Center;				// ��ü ���� �߽�
	Vector3 SectionTotalSize;	// ��ü ���� ũ��
	Vector3 Min;				// ��ü ���������� �ּҰ�
	Vector3 Max;				// ��ü ���������� �ִ밪
	int CountX;					// �� ���� ���ҵǾ� �ִ���
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

