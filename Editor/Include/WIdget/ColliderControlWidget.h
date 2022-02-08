#pragma once

#include "ComponentControlWidget.h"

class CColliderControlWidget :
	public CComponentControlWidget
{
	friend class CIMGUIWindow;
	friend class CDetailWindow;

protected:
	CColliderControlWidget();
	virtual ~CColliderControlWidget();

public:
	virtual bool Init() override;

public:
	void OnSelectColliderType(int idx, const char* label);
	void OnChangeOffsetWidget(float val[3]);
	void OnSelectCollisionProfile(int idx, const char* label);
	void OnClickEditProfile();
	void OnChangeBoxWidth(float val);
	void OnChangeBoxHeight(float val);
	void OnChangeCircleRadius(float val);

private:
	void changeType(eColliderType type);

protected:
	class CIMGUIComboBox* mColliderTypeWidget;
	class CIMGUIInputFloat3* mOffsetWidget;
	class CIMGUIComboBox* mCollsionProfileWidget;
	class CIMGUIButton* mEditCollisionProfileWidget;
	class CIMGUIText* mTypeText;
	class CIMGUIInputFloat* mEditBox2DWidth;
	class CIMGUIInputFloat* mEditBox2DHeight;
	class CIMGUIInputFloat* mEditCircleRadius;
	class CIMGUISeperator* mSeperator;

	eColliderType meType;
    static int mWindowID;
};

