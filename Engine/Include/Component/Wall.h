#pragma once

#include "../GameInfo.h"

class CWall
{
	friend class CWallComponent;

private:
	CWall();
	CWall(const CWall& wall);
	~CWall();

public:
	void Start();
	void Update();

public:
	void Save(FILE* fp);
	void Load(FILE* fp);

public:
	void SetInfo(class CWallComponent* owner, class CTile* parent, eTileShape shape, const Vector3& renderPos, const Vector2& size);
	void SetShape(eTileShape eShape)
	{
		meTileShape = eShape;
	}

	void SetRenderPos(const Vector3& pos);
	void SetRenderPos(const float x, const float y);
	void SetSize(const Vector2& size);
	void SetSize(const float x, const float y);

	void SetFrameStart(const Vector2& start)
	{
		mFrameStart = start;
	}

	void SetFrameStart(const float x, const float y)
	{
		mFrameStart = Vector2(x, y);
	}

	void SetFrameEnd(const Vector2& end)
	{
		mFrameEnd = end;
	}

	void SetFrameEnd(const float x, const float y)
	{
		mFrameEnd = Vector2(x, y);
	}

	void SetOpacity(const float opactiy)
	{
		mOpacity = opactiy;
	}

	void SetRender(bool bRender)
	{
		mbRender = bRender;
	}

	void SetSortY(const float y)
	{
		mSortY = y;
	}

	void AddSortY(const float y)
	{
		mSortY += y;
	}

	void SubSortY(const float y)
	{
		mSortY -= y;
	}

public:
	eTileShape GetTileShape() const
	{
		return meTileShape;
	}

	const Matrix& GetWorldMatrix() const
	{
		return mMatWorld;
	}

	const Vector3& GetRenderPos() const
	{
		return mRenderPos;
	}

	const Vector2& GetSize() const
	{
		return mSize;
	}

	const Vector2& GetFrameStart() const
	{
		return mFrameStart;
	}

	const Vector2& GetFrameEnd() const
	{
		return mFrameEnd;
	}

	const float GetOpacity() const
	{
		return mOpacity;
	}

	float GetSortY() const
	{
		return mSortY;
	}

private:
	void makeWorldMatrix();

private:
	class CWallComponent* mOwner;
	class CTile* mParentTile;
	bool mbRender;
	eTileShape meTileShape;
	Vector2 mSize;
	Vector3 mRenderPos;
	float mSortY;
	Matrix mMatWorld;
	Vector2 mFrameStart;
	Vector2 mFrameEnd;
	float mOpacity;
};

