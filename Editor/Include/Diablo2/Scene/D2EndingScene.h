#pragma once

#include "Scene/SceneMode.h"
#include "../UI/D2EndingWidget.h"

class CD2EndingScene :
    public CSceneMode
{
public:
    CD2EndingScene();
    ~CD2EndingScene();

public:
    virtual bool Init() override;

private:
    bool loadWindow();
    void createKey();
    void onESCDown(float deltaTime);

private:
    CSharedPtr<CD2EndingWidget> mEndingWindow;
};

