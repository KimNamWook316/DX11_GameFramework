#pragma once
#include "IMGUIWidget.h"
class CIMGUIDummy :
    public CIMGUIWidget
{
public:
    CIMGUIDummy();
    virtual ~CIMGUIDummy();
    
public:
    virtual bool Init() override;
    virtual void Render() override;
};

