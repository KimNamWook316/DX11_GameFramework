#pragma once

#include "Scene/SceneMode.h"

class CMainScene :
    public CSceneMode
{
public:
    CMainScene();
    ~CMainScene();

public:
    virtual bool Init();
};

