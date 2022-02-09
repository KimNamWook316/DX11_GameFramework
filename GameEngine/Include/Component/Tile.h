#pragma once

#include "../Engine.h"
#include "../Animation/AnimationSequence2DInstance.h"

class CTile
{
	friend class CTileMapComponent;

private:
	CTile();
	CTile(const CTile& tile);
	~CTile();

public:
	void Update(float deltaTime);

public:
	void AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop = true,
		const float playTime = 1.f, const float playScale = 1.f, bool bIsReverse = false);
	void AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name,
		bool bIsLoop = true, const float playTime = 1.f, const float playScale = 1.f, bool bIsReverse = false);
	bool CheckCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	void SetPlayTime(const std::string& name, const float playTime);
	void SetPlayScale(const std::string& name, const float playScale);
	void SetLoop(const std::string& name, bool bLoop);
	void SetReverse(const std::string& name, bool bReverse);
	void SetCurrentAnimation(const std::string& name);

public:
	eTileType GetTileType() const
	{
		return meTileType;
	}

	eTileShape GetShape() const
	{
		return meShape;
	}

	const Vector3& GetSize() const
	{
		return mSize;
	}

	const Vector3& GetCenter() const
	{
		return mCenter;
	}

	const Vector3& GetPos() const
	{
		return mPos;
	}

	const Matrix& GetWorldMatrix() const
	{
		return mMatWorld;
	}

	int GetIndexX() const
	{
		return mIndexX;
	}

	int GetIndexY() const
	{
		return mIndexY;
	}

	int GetIndex() const
	{
		return mIndex;
	}

private:
	class CTileMapComponent* mOwner;
	eTileShape meShape;
	eTileType meTileType;
	Vector3 mPos;
	Vector3 mSize;
	Vector3 mCenter;
	Matrix mMatWorld;
	CAnimationSequence2DInstance* mAnimInstance;
	int mIndexX;
	int mIndexY;
	int mIndex;
};

