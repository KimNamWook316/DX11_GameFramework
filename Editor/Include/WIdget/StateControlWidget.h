#pragma once

#include "ComponentControlWidget.h"

class CStateControlWidget :
    public CComponentControlWidget
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CStateControlWidget();
    virtual ~CStateControlWidget();

public:
    virtual bool Init() override;

public:
    void OnClickSetInitialState();

private:
    class CIMGUIComboBox* mInitialStateComoboBox;
    class CIMGUIButton* mSetInitialStateButton;
};

