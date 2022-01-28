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
    void OnPosXInputChanged(float val);
    void OnPosYInputChanged(float val);
    void OnPosZInputChanged(float val);

private:
    class CComponentControlWidget* CreateControlWidget(class CComponent* comp);

private:
    class CIMGUIInputFloat* mPosXInput;
    class CIMGUIInputFloat* mPosYInput;
    class CIMGUIInputFloat* mPosZInput;
};

