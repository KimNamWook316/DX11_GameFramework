#pragma once
#include "IMGUIWidget.h"
class CIMGUIDummy :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;
    friend class CIMGUIChild;

protected:
    CIMGUIDummy();
    virtual ~CIMGUIDummy();
    
public:
    virtual bool Init() override;
    virtual void Render() override;
};

