#pragma once

#include "IMGUIWindow.h"

class CEditorMenuWindow :
    public CIMGUIWindow
{
public:
    CEditorMenuWindow();
    virtual ~CEditorMenuWindow();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;

public:
    void OnClickCreateObject();
    void OnClickCreateComponent();
    void OnClickSaveScene();
    void OnClickLoadScene();

private:
    class CIMGUIButton* mCreateObjectButton;
    class CIMGUIButton* mCreateComponentButton;

    class CIMGUITextInput* mObjectNameInput;
    class CIMGUITextInput* mComponentNameInput;

    class CIMGUIComboBox* mCreatableObjectsComboBox;
    class CIMGUIComboBox* mCreatableComponentsComboBox;
};

