#pragma once

#include "IMGUIWindow.h"

class CIMGUITestWindow :
    public CIMGUIWindow
{
public:
    CIMGUITestWindow();
    virtual ~CIMGUITestWindow();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    
public:
    void ClickButton1();
    void ClickButton2();
};

