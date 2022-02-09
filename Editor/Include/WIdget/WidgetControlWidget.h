#pragma once

#include "ComponentControlWidget.h"

class CWidgetControlWidget :
    public CComponentControlWidget
{
	friend class CIMGUIWindow;
	friend class CDetailWindow;

protected:
	CWidgetControlWidget();
	virtual ~CWidgetControlWidget();

public:
	virtual bool Init() override;

protected:
	class CIMGUIComboBox* mWidgetType;
	class CIMGUIButton* mCreateWindowButton;
	
};

