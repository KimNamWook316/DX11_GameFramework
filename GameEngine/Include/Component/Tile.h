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
	void Start();
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
	void Save(FILE* fp);
	void Load(FILE* fp);

public:
	eTileType GetTileType() const
	{
		return meTileType;
	}

	eTileShape GetShape() const
	{
		return meShape;
	}

	const Vector2& GetSize() const
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

	float GetOpacity() const
	{
		return mOpacity;
	}
	
	Vector2 GetFrameStart() const
	{
		return mFrameStart;
	}

	Vector2 GetFrameEnd() const
	{
		return mFrameEnd;
	}

	bool GetIsRender() const
	{
		return mFrameStart.x != 0.f || mFrameStart.y != 0.f || mFrameEnd.x != 0.f || mFrameEnd.y != 0.f;
	}

public:
	void SetIndex(const int idxX, const int idxY, const int idx)
	{
		mIndexX = idxX;
		mIndexY = idxY;
		mIndex = idx;
	}

	void SetShape(eTileShape eShape)
	{
		meShape = eShape;
	}

	void SetTileType(eTileType type)
	{
		meTileType = type;
	}

	void SetPos(const Vector3& pos)
	{
		mPos = pos;
	}

	void SetPos(const float x, const float y, const float z)
	{
		mPos = Vector3(x, y, z);
	}

	void SetSize(const Vector2& size)
	{
		mSize = size;
	}

	void SetSize(const float x, const float y)
	{
		mSize = Vector2(x, y);
	}

	void SetOpacity(const float opacity)
	{
		mOpacity = opacity;
	}

	void SetFrameStart(const Vector2& frame)
	{
		mFrameStart = frame;
	}

	void SetFrameStart(const float x, const float y)
	{
		mFrameStart = Vector2(x, y);
	}

	void SetFrameEnd(const Vector2& frame)
	{
		mFrameEnd = frame;
	}

	void SetFrameEnd(const float x, const float y)
	{
		mFrameEnd = Vector2(x, y);
	}

	void SetRenderWall(bool bRender);
	void AddWall(class CWall* wall);

private:
	class CTileMapComponent* mOwner;
	eTileShape meShape;
	eTileType meTileType;
	Vector3 mPos;	// Left Bottom
	Vector2 mSize;
	Vector3 mCenter;
	Matrix mMatWorld;
	CAnimationSequence2DInstance* mAnimInstance;
	int mIndexX;
	int mIndexY;
	int mIndex;
	Vector2 mFrameStart;
	Vector2 mFrameEnd;
	float mOpacity;
	std::vector<class CWall*> mVecWall;
};

