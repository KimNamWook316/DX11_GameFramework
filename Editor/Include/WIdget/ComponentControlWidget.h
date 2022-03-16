#pragma once

#include "IMGUIWidgetList.h"

class CComponentControlWidget :
    public CIMGUIWidgetList
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CComponentControlWidget();
    virtual ~CComponentControlWidget();

public:
    virtual bool Init() override;
    virtual void Render() override;

protected:
    CSharedPtr<class CComponent> mComponent;
};

