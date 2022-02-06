#pragma once

#include "IMGUIWidgetList.h"

class CComponentControlWidget :
    public CIMGUIWidgetList
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CComponentControlWidget();
    virtual ~CComponentControlWidget() = 0;

public:
    virtual bool Init() override;
    virtual void Render() override;

protected:
    class CComponent* mComponent;
};
