#pragma once

#include "Scene/SceneMode.h"
#include "../UI/D2LoadingWindow.h"
#include "../D2Info.h"

class CD2LoadingScene :
    public CSceneMode
{
public:
    CD2LoadingScene();
    ~CD2LoadingScene();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;

private:
    bool loadWindow();

private:
    CSharedPtr<CD2LoadingWindow> mLoadingWindow;
    class CD2LoadingThread* mLoadingThread;
};

