#include "Wall.h"
#include "Tile.h"
#include "WallComponent.h"

CWall::CWall()	:
	meTileShape(eTileShape::Rect),
	mOpacity(1.f),
	mOwner(nullptr),
	mParentTile(nullptr),
	mbRender(false)
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
	SetSortY(renderPos.y);
	SetSize(size);
	parent->AddWall(this);
}

void CWall::SetRenderPos(const Vector3& pos)
{
	mRenderPos = pos;
	makeWorldMatrix();
}

void CWall::SetRenderPos(const float x, const float y)
{
	mRenderPos = Vector3(x, y, 0.f);
	makeWorldMatrix();
}

void CWall::SetSize(const Vector2& size)
{
	mSize = size;
	makeWorldMatrix();
}

void CWall::SetSize(const float x, const float y)
{
	mSize = Vector2(x, y);
	makeWorldMatrix();
}

void CWall::makeWorldMatrix()
{
	Vector3 ownerPos = mOwner->GetWorldPos();
	Vector3 pos = ownerPos + mRenderPos;

	Matrix matScale, matTranslate;
	matScale.Scaling(mSize.x, mSize.y, 1.f);
	matTranslate.Translation(mRenderPos);

	mMatWorld = matScale * matTranslate;
}
