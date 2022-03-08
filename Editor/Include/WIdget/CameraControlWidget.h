#pragma once

#include "ComponentControlWidget.h"

class CCameraControlWidget :
    public CComponentControlWidget
{
	friend class CIMGUIWindow;
	friend class CDetailWindow;

protected:
	CCameraControlWidget();
	virtual ~CCameraControlWidget();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void OnChangeScale(float val[3]);
	void OnChangePos(float val[3]);
	void OnClickReset();
	void OnSelectCameraType(int idx, const char* label);
	void OnCheckIsPlayer(int idx, bool bCheck);
	void OnChangeViewPortRatioX(int val);
	void OnChangeViewPortRatioY(int val);
	void OnClickViewPortCenter();

private:
	class CIMGUIInputFloat3* mScale;
	class CIMGUIInputFloat3* mPos;
	class CIMGUIComboBox* mCameraType;
	class CIMGUICheckBox* mIsPlayer;
	class CIMGUISliderInt* mViewPortRatioX;
	class CIMGUISliderInt* mViewPortRatioY;
	class CIMGUIButton* mViewPortCenter;
	class CIMGUIButton* mResetButton;
};

