#include "AnimationSequence2DData.h"
#include "../Resource/Animation/AnimationSequence2D.h"

CAnimationSequence2DData::CAnimationSequence2DData()	:
	mFrame(0),
	mTime(0.f),
	mFrameTime(0.f),
	mPlayTime(0.f),
	mPlayScale(1.f),
	mbIsLoop(false),
	mbIsReverse(false)
{
}

CAnimationSequence2DData::~CAnimationSequence2DData()
{
	auto iter = mVecNotify.begin();
	auto iterEnd = mVecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

void CAnimationSequence2DData::Replay()
{
	mFrame = 0;
	mTime = 0;
}
