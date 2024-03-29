#include "PlayerAnimation2D.h"

CPlayerAnimation2D::CPlayerAnimation2D()
{
}

CPlayerAnimation2D::CPlayerAnimation2D(const CPlayerAnimation2D& anim)	:
	CAnimationSequence2DInstance(anim)
{
}

CPlayerAnimation2D::~CPlayerAnimation2D()
{
}

bool CPlayerAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
	{
		assert(false);
		return false;
	}

	AddAnimation("PlayerIdle", "Idle");

	return true;
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}
