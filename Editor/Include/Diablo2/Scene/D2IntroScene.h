#pragma once

#include "Scene/SceneMode.h"
#include "../UI/D2IntroWindow.h"

class CD2IntroScene :
    public CSceneMode
{
public:
    CD2IntroScene();
    ~CD2IntroScene();

public:
    virtual bool Init() override;

private:
    bool loadWindow();
    bool loadSound();

private:
    CSharedPtr<CD2IntroWindow> mIntroWindow;
    CSharedPtr<class CSound> mIntroMusic;
};

