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

	AddAnimation("PlayerIdle_Front", "Idle_Front");
	AddAnimation("PlayerIdle_Back", "Idle_Back");
	AddAnimation("PlayerMove_Front", "Move_Front");
	AddAnimation("PlayerMove_Back", "Move_Back");
	AddAnimation("PlayerAttack", "Attack");
	return true;
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}
