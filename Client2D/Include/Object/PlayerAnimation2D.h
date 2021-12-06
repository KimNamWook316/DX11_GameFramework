#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CPlayerAnimation2D :
    public CAnimationSequence2DInstance
{
public:
    CPlayerAnimation2D();
    CPlayerAnimation2D(const CPlayerAnimation2D& anim);
    ~CPlayerAnimation2D();

public:
    virtual bool Init();
    virtual CPlayerAnimation2D* Clone();
};

