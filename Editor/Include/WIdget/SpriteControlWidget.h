#pragma once

#include "ComponentControlWidget.h"

class CSpriteControlWidget :
    public CComponentControlWidget 
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CSpriteControlWidget();
    virtual ~CSpriteControlWidget();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    /*
        Sprite 
        BaseColor
        Opacity
        Animation
    */
};

