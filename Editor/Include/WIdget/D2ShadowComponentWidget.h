#pragma once

#include "ComponentControlWidget.h"

class CD2ShadowComponentWidget :
    public CComponentControlWidget
{
	friend class CIMGUIWindow;
	friend class CDetailWindow;

protected:
	CD2ShadowComponentWidget();
	virtual ~CD2ShadowComponentWidget();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void OnChangeOffset(float val[2]);

private:
	class CIMGUIInputFloat2* mOffset;
};

