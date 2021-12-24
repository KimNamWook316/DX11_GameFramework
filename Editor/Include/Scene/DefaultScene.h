#pragma once

#include "Scene\SceneMode.h"

class CDefaultScene :
    public CSceneMode
{
public:
    CDefaultScene();
    ~CDefaultScene();

public:
    virtual bool Init();

private:
    bool loadAnimationSequence2D();
};

