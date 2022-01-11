#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/MainWidget.h"

class CMainScene :
    public CSceneMode
{
public:
    CMainScene();
    ~CMainScene();

public:
    virtual bool Init();

private:
    void createMaterial();
    void createAnimationSequence();

private:
    CSharedPtr<CMainWidget> mMainWidget;
};

