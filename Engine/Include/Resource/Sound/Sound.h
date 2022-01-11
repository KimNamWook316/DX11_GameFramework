#pragma once

#include "../../Ref.h"

class CSound :
    public CRef
{
    friend class CSoundManager;

private:
    CSound();
    ~CSound();
};

