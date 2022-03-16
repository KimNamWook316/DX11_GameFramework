#pragma once

#include "ComponentControlWidget.h"

class CSceneComponentControlWidget :
    public CComponentControlWidget
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CSceneComponentControlWidget();
    virtual ~CSceneComponentControlWidget();

public:
    virtual bool Init() override;

protected:

};

