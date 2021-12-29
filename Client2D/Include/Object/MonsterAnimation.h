#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CMonsterAnimation :
    public CAnimationSequence2DInstance
{
public:
    CMonsterAnimation();
    CMonsterAnimation(const CMonsterAnimation& anim);
    virtual ~CMonsterAnimation();

public:
    virtual bool Init() override;
    virtual CMonsterAnimation* Clone();
};

