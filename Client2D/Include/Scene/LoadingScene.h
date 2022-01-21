#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/LoadingWidget.h"

class CLoadingScene :
    public CSceneMode
{
public:
    CLoadingScene();
    ~CLoadingScene();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;

private:
    CSharedPtr<CLoadingWidget> mLoadingWidget;
    class CLoadingThread* mLoadingThread;
};

