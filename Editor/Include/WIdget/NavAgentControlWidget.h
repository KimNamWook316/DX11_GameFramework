#pragma once

#include "ComponentControlWidget.h"

class CNavAgentControlWidget :
    public CComponentControlWidget 
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CNavAgentControlWidget();
    virtual ~CNavAgentControlWidget();

public:
    virtual bool Init() override;

private:

};

