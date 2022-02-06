#pragma once

#include "IMGUIWindow.h"

class CDetailWindow :
    public CIMGUIWindow
{
public:
    CDetailWindow();
    virtual ~CDetailWindow();

public:
    virtual bool Init();
    virtual void Update(float deltaTime);

public:
    void UpdateComponentUI(class CGameObject* obj);

private:
    class CComponentControlWidget* CreateControlWidget(class CComponent* comp);
};

