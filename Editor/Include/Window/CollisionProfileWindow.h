#pragma once

#include "IMGUIWindow.h"

class CCollisionProfileWindow :
    public CIMGUIWindow
{
public:
    CCollisionProfileWindow();
    virtual ~CCollisionProfileWindow();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime);

public:
    void OnSelectList(int idx, const char* label);
    void OnClickCreate();
    void OnCheckInteraction(int idx, bool bCheck);
    void OnClickSave();

protected:
    class CIMGUIListBox* mProfileList;
    class CIMGUIComboBox* mChannelList;
    class CIMGUITextInput* mProfileNameInput;
    class CIMGUIButton* mCreateProfileButton;
    class CIMGUITextInput* mProfileChannel;
    class CIMGUICheckBox* mInteractionEdit;
    class CIMGUIButton* mSaveButton;
};

