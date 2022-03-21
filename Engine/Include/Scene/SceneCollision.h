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
		CountZ(1),
		SectionSize(1000.f, 1000.f, 1.f),
		SectionTotalSize(1000.f, 1000.f, 1.f),
		Min(-500.f, -500.f, -0.5f),
		Max(500.f, 500.f, 0.5f)
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
	void DoCollide(float deltaTime);
	void DoCollideMouse(float deltaTime);
	bool DoCollideWidget(float deltaTime);

public:
	void SetSectionSize(const Vector3& size);
	void SetSectionSize(const float x, const float y, const float z);
	void SetSectionCenter(const Vector3& center);
	void SetSectionCenter(const float x, const float y, const float z);
	void SetSectionMin(const Vector3& min);
	void SetSectionMin(const float x, const float y, const float z);
	void SetSectionMax(const Vector3& max);
	void SetSectionMax(const float x, const float y, const float z);
	void SetSectionCount(const int countX, const int countY, const int countZ = 1);
	void CreateSection();
	void Clear();
	void AddCollider(class CColliderComponent* collider);
	void DeleteCollider(class CColliderComponent* collider);

public:
	class CColliderComponent* GetMouseCollision()
	{
		return mMouseCollision;
	}

private:
	void classifyColliderBySection();

private:
	class CScene* mScene;
	CollisionSectionInfo* mSectionInfo;
	std::list<CSharedPtr<class CColliderComponent>> mColliderList;
	CSharedPtr<class CColliderComponent> mMouseCollision;

	bool mbWidgetClicked;
};

