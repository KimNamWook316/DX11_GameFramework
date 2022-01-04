#include "MonsterAnimation.h"

CMonsterAnimation::CMonsterAnimation()
{
	SetTypeID<CMonsterAnimation>();
}

CMonsterAnimation::CMonsterAnimation(const CMonsterAnimation& anim)
{
}

CMonsterAnimation::~CMonsterAnimation()
{
}

bool CMonsterAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
	{
		assert(false);
		return false;
	}

	AddAnimation("PlayerIdle_Front", "Idle_Front");
 //	AddAnimation("PlayerIdle_Back", "Idle_Back");
 //	AddAnimation("PlayerMove_Front", "Move_Front");
 //	AddAnimation("PlayerMove_Back", "Move_Back");
 //	AddAnimation("PlayerAttack", "Attack");
	return true;
}

CMonsterAnimation* CMonsterAnimation::Clone()
{
	return new CMonsterAnimation(*this);
}
