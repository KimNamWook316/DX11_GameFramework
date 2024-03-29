#pragma once

#include "IMGUIWidget.h"

class CIMGUILabel :
	public CIMGUIWidget
{
public:
	CIMGUILabel();
	virtual ~CIMGUILabel();

public:
	virtual bool Init() override;
	virtual void Render() override;
	
public:
	void SetAlign(const float x, const float y)
	{
		mAlign = ImVec2(x, y);
	}

protected:
	ImVec2 mAlign;
};

