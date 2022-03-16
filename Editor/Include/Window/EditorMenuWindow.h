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
    void OnClickPlay();
    void OnClickSaveObject();
    void OnClickLoadObject();
    void OnClickInstanciate();

    void OnClickOpenCollisionProfileEditor();
    void OnClickOpenTileSetEditor();

private:
    void refreshPrefabList();

private:
    class CIMGUIMainMenuBar* mMainMenuBar;

    class CIMGUIButton* mCreateObjectButton;
    class CIMGUIButton* mCreateComponentButton;
    class CIMGUIButton* mInstanciatePrefabButton;

    class CIMGUITextInput* mObjectNameInput;
    class CIMGUITextInput* mComponentNameInput;

    class CIMGUIComboBox* mCreatableObjectsComboBox;
    class CIMGUIComboBox* mCreatableComponentsComboBox;
    class CIMGUIListBox* mCreatablePrefabsListBox;

    class CIMGUIMenuItem* mOpenCollsionProfileMenuButton;
    class CIMGUIMenuItem* mOpenTileSetEditorMenuButton;
};

