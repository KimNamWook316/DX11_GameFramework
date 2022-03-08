#include "Wall.h"
#include "Tile.h"
#include "WallComponent.h"

CWall::CWall()	:
	meTileShape(eTileShape::Rect),
	mOpacity(1.f),
	mOwner(nullptr),
	mParentTile(nullptr),
	mbRender(false),
	mSortY(0.f)
{
}

CWall::CWall(const CWall& wall)
{
	*this = wall;
	mParentTile = nullptr;
}

CWall::~CWall()
{
}

void CWall::Start()
{
	makeWorldMatrix();
}

void CWall::Update()
{
	Vector3 ownerPos = mOwner->GetWorldPos();
	Vector3 pos = ownerPos + mRenderPos;

	pos.z = mSortY / 30000.f * 1000.f;

	Matrix matScale, matTranslate;
	matScale.Scaling(mSize.x, mSize.y, 1.f);
	matTranslate.Translation(pos);

	mMatWorld = matScale * matTranslate;
}

void CWall::Save(FILE* fp)
{
}

void CWall::Load(FILE* fp)
{
}

void CWall::SetInfo(class CWallComponent* owner, CTile* parent, eTileShape shape, const Vector3& renderPos, const Vector2& size)
{
	mOwner = owner;
	mParentTile = parent;
	SetShape(shape);
	SetRenderPos(renderPos);
	SetSortY(renderPos.y + 40.f);
	SetSize(size);
	parent->AddWall(this);
}

void CWall::SetRenderPos(const Vector3& pos)
{
	mRenderPos = pos;
}

void CWall::SetRenderPos(const float x, const float y)
{
	mRenderPos = Vector3(x, y, 0.f);
}

void CWall::SetSize(const Vector2& size)
{
	mSize = size;
}

void CWall::SetSize(const float x, const float y)
{
	mSize = Vector2(x, y);
}

void CWall::makeWorldMatrix()
{
	Vector3 ownerPos = mOwner->GetWorldPos();
	Vector3 pos = ownerPos + mRenderPos;

	pos.z = mSortY / 30000.f * 1000.f;

	Matrix matScale, matTranslate;
	matScale.Scaling(mSize.x, mSize.y, 1.f);
	matTranslate.Translation(pos);

	mMatWorld = matScale * matTranslate;
}
