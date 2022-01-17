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

	return true;
}

CMonsterAnimation* CMonsterAnimation::Clone()
{
	return new CMonsterAnimation(*this);
}
