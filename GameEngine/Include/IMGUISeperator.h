#pragma once

#include "IMGUIWidget.h"

class CIMGUISeperator :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;
    friend class CIMGUIChild;

protected:
    CIMGUISeperator();
    virtual ~CIMGUISeperator();
    
public:
    virtual bool Init() override;
    virtual void Render() override;
};

