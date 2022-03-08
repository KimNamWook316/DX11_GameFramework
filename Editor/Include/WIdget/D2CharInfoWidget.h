#pragma once

#include "ComponentControlWidget.h"

class CD2CharInfoWidget :
    public CComponentControlWidget
{
    friend class CIMGUIWindow;
    friend class CDetailWindow;

protected:
    CD2CharInfoWidget();
    virtual ~CD2CharInfoWidget();

public:
    virtual bool Init() override;

public:
    void OnSelectDataSet(int idx, const char* label);

private:
    class CIMGUIComboBox* mDataSet;
    class CIMGUIText* mName;
    class CIMGUIInputFloat* mSpeed;
    class CIMGUIInputFloat* mMaxSpeed;
    class CIMGUIInputFloat2* mDir;
    class CIMGUITextInput* mSpriteDir;
};

