#include "AnimationSequence2DInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance()	:
	mScene(nullptr),
	mOwner(nullptr)
{
}

CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim)
{
}

CAnimationSequence2DInstance::~CAnimationSequence2DInstance()
{
	auto iter = mMapAnimation.begin();
	auto iterEnd = mMapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimationSequence2DInstance::Update(float deltaTime)
{

}

CAnimationSequence2DInstance* CAnimationSequence2DInstance::Clone()
{
	return new CAnimationSequence2DInstance(*this);
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	CAnimationSequence2DData* anim = FindAnimation(name);

	if (anim)
	{
		return;
	}

	anim = new CAnimationSequence2DData;

	CAnimationSequence2D* sequence = nullptr;

	if (mScene)
	{

	}
	else
	{
	}

	anim->mSequence = sequence;
	anim->mbIsLoop = bIsLoop;
	anim->mPlayTime = playTime;
	anim->mPlayScale = playScale;
	anim->mbIsReverse = bIsReverse;
}

void CAnimationSequence2DInstance::ChangeAnimation(const std::string& name)
{
}

bool CAnimationSequence2DInstance::CheckCurrentAnimation(const std::string& name)
{
	return false;
}

void CAnimationSequence2DInstance::SetPlayTime(const std::string& name, const float playTime)
{
}

void CAnimationSequence2DInstance::SetPlayScale(const std::string& name, const float playScale)
{
}

void CAnimationSequence2DInstance::SetLoop(const std::string& name, bool bLoop)
{
}

void CAnimationSequence2DInstance::SetReverse(const std::string& name, bool bReverse)
{
}

void CAnimationSequence2DInstance::SetCurrentAnimation(const std::string& name)
{
}

CAnimationSequence2DData* CAnimationSequence2DInstance::FindAnimation(const std::string& name)
{
	auto iter = mMapAnimation.find(name);

	if (iter == mMapAnimation.end())
	{
		return nullptr;
	}

	return iter->second;
}
