#pragma once

#include "IMGUIWidgetList.h"

class CSpriteControlWidget :
    public CIMGUIWidgetList
{
    friend class CIMGUIWindow;

public:
    CSpriteControlWidget();
    virtual ~CSpriteControlWidget();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    class CSpriteComponent* mSpriteComponent;

    /*
        Sprite 
        BaseColor
        Opacity
        Animation
    */
};

