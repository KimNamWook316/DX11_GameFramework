#pragma once

#include "Scene/SceneMode.h"

class CTestScene :
    public CSceneMode
{
public:
    CTestScene();
    ~CTestScene();

public:
    virtual bool Init() override;
};

