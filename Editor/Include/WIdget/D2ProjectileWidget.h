#pragma once

#include "ComponentControlWidget.h"

class CD2ProjectileWidget :
    public CComponentControlWidget
{
	friend class CIMGUIWindow;
	friend class CDetailWindow;

protected:
	CD2ProjectileWidget();
	virtual ~CD2ProjectileWidget();

public:
	virtual bool Init() override;

public:
	void OnSelectSkillInfo(int idx, const char* label);

private:
	class CIMGUIComboBox* mSkillInfo;
};

