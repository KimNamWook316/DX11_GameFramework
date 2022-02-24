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

public:
	void Save(FILE* fp);
	void Load(FILE* fp);

public:
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

private:
	void makeWorldMatrix();

private:
	class CWallComponent* mOwner;
	class CTile* mParentTile;
	bool mbRender;
	eTileShape meTileShape;
	Vector2 mSize;
	Vector3 mRenderPos;
	Matrix mMatWorld;
	Vector2 mFrameStart;
	Vector2 mFrameEnd;
	float mOpacity;
};

