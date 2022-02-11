#include "Tile.h"
#include "TileMapComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"

CTile::CTile()	:
	meShape(eTileShape::Rect),
	meTileType(eTileType::Normal),
	mAnimInstance(nullptr),
	mOwner(nullptr),
	mIndexX(-1),
	mIndexY(-1),
	mIndex(-1),
	mOpacity(1.f)
{
}

CTile::CTile(const CTile& tile)
{
	*this = tile;

	mAnimInstance = tile.mAnimInstance->Clone();
}

CTile::~CTile()
{
	SAFE_DELETE(mAnimInstance);
}

void CTile::Start()
{
	mCenter = mPos + Vector3(mSize.x / 2.f, mSize.y / 2.f, 0.f);
}

void CTile::Update(float deltaTime)
{
	if (mAnimInstance)
	{
		mAnimInstance->Update(deltaTime);

		int frame = mAnimInstance->GetCurrentAnimation()->GetCurrentFrame();
		mFrameStart = mAnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(frame).Start;
		mFrameEnd = mFrameStart + mAnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(frame).Size;
	}

	Vector3 ownerPos = mOwner->GetWorldPos();
	Vector3 pos = ownerPos + mPos;

	Matrix matScale, matTranslate;
	matScale.Scaling(mSize.x, mSize.y, 1.f);
	matTranslate.Translation(mPos);

	mMatWorld = matScale * matTranslate;
}

void CTile::AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	if (!mAnimInstance)
	{
		mAnimInstance = new CAnimationSequence2DInstance;
	}

	mAnimInstance->AddAnimation(sequenceName, name, bIsLoop, playTime, playScale, bIsReverse);
}

void CTile::AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	if (!mAnimInstance)
	{
		mAnimInstance = new CAnimationSequence2DInstance;
	}

	mAnimInstance->AddAnimation(fileName, pathName, name, bIsLoop, playTime, playScale, bIsReverse);
}

bool CTile::CheckCurrentAnimation(const std::string& name)
{
	if (!mAnimInstance)
	{
		return false;
	}

	return mAnimInstance->CheckCurrentAnimation(name);
}

void CTile::ChangeAnimation(const std::string& name)
{
	if (!mAnimInstance)
	{
		return;
	}

	return mAnimInstance->ChangeAnimation(name);
}

void CTile::SetPlayTime(const std::string& name, const float playTime)
{
	if (!mAnimInstance)
	{
		return;
	}

	return mAnimInstance->SetPlayTime(name, playTime);
}

void CTile::SetPlayScale(const std::string& name, const float playScale)
{
	if (!mAnimInstance)
	{
		return;
	}

	return mAnimInstance->SetPlayScale(name, playScale);
}

void CTile::SetLoop(const std::string& name, bool bLoop)
{
	if (!mAnimInstance)
	{
		return;
	}

	return mAnimInstance->SetLoop(name, bLoop);
}

void CTile::SetReverse(const std::string& name, bool bReverse)
{
	if (!mAnimInstance)
	{
		return;
	}

	return mAnimInstance->SetReverse(name, bReverse);
}

void CTile::SetCurrentAnimation(const std::string& name)
{
	if (!mAnimInstance)
	{
		return;
	}

	return mAnimInstance->SetCurrentAnimation(name);
}

void CTile::Save(FILE* fp)
{
}

void CTile::Load(FILE* fp)
{
}
