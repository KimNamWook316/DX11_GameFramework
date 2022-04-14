#pragma once

#include "Scene/SceneMode.h"

class CD2MainScene :
    public CSceneMode
{
public:
    CD2MainScene();
    ~CD2MainScene();

public:
    virtual bool Init();

public:
    template<typename T>
    void SetLoadingFunction(T* obj, void(T::* func)(bool, float))
    {
        mLoadingFunction = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
    }

public:
    void OnLoadEnd();

#ifdef _DEBUG
    void OnF2Down(float deltaTime);
#endif // _DEBUG

private:
    void createKey();
    void createMesh();
    void createShader();
    void loadSound();

private:
    std::function<void(bool, float)> mLoadingFunction;
    CSharedPtr<class CSound> mMainMusic;
};

