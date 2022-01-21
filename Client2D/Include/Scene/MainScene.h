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

public:
    template <typename T>
    void SetLoadingFunction(T* obj, void(T::* func)(bool, float))
    {
        mLoadingFunction = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
    }

private:
    void createMaterial();
    void createAnimationSequence();

private:
    CSharedPtr<CMainWidget> mMainWidget;
    std::function<void(bool, float)> mLoadingFunction; 
};

