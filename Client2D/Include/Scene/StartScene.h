#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/StartWidget.h"

class CStartScene :
    public CSceneMode
{
public:
    CStartScene();
    ~CStartScene();

public:
    virtual bool Init() override;

private:
    CSharedPtr<CStartWidget> mStartWidget;
};

