#include "RenderState.h"

CRenderState::CRenderState()	:
	mState(nullptr),
	mPrevState(nullptr)
{
}

CRenderState::~CRenderState()
{
	SAFE_RELEASE(mState);
	SAFE_RELEASE(mPrevState);
}
