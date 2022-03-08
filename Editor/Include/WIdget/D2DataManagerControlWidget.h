#pragma once

#include "ComponentControlWidget.h"

class CD2DataManagerControlWidget :
	public CComponentControlWidget
{
	friend class CIMGUIWindow;
	friend class CDetailWindow;

protected:
	CD2DataManagerControlWidget();
	virtual ~CD2DataManagerControlWidget();

public:
	virtual bool Init() override;
};

