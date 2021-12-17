#include "EditorMenuWindow.h"
#include "IMGUIButton.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUISeperator.h"
#include "IMGUIListBox.h"
#include "../EditorInfo.h"
#include "Scene/SceneManager.h"
#include "ObjectHierachyWindow.h"
#include "IMGUIManager.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"

CEditorMenuWindow::CEditorMenuWindow()  :
    mCreateObjectButton(nullptr),
    mCreateComponentButton(nullptr),
    mObjectNameInput(nullptr),
    mComponentNameInput(nullptr),
    mCreatableObjectsComboBox(nullptr),
    mCreatableComponentsComboBox(nullptr)
{
}

CEditorMenuWindow::~CEditorMenuWindow()
{
}

bool CEditorMenuWindow::Init()
{
    CIMGUIWindow::Init();

    CIMGUIText* text = AddWidget<CIMGUIText>("text");
    text->SetText("Create Object");

    mCreatableObjectsComboBox = AddWidget<CIMGUIComboBox>("Object List", 150.f, 0.f);
    mCreatableObjectsComboBox->AddItem("GameObject");
    mCreatableObjectsComboBox->AddItem("Player");

    mObjectNameInput = AddWidget<CIMGUITextInput>("Object Name", 100.f, 0.f);
    
    mCreateObjectButton = AddWidget<CIMGUIButton>("Create Object", 0.f, 0.f);
    mCreateObjectButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickCreateObject);

    AddWidget<CIMGUISeperator>("seperator");

    text = AddWidget<CIMGUIText>("text");
    text->SetText("Create Components");

    mCreatableComponentsComboBox = AddWidget<CIMGUIComboBox>("Component List", 150.f, 0.f);
    mCreatableComponentsComboBox->AddItem("SpriteComponent");
    mCreatableComponentsComboBox->AddItem("StaticMeshComponent");

    mComponentNameInput = AddWidget<CIMGUITextInput>("Component Name", 100.f, 0.f);
    
    mCreateComponentButton = AddWidget<CIMGUIButton>("Create Component", 0.f, 0.f);
    mCreateComponentButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickCreateComponent);
    return true;
}

void CEditorMenuWindow::Update(float deltaTime)
{
    CIMGUIWindow::Update(deltaTime);
}

void CEditorMenuWindow::OnClickCreateObject()
{
    int selectIdx = mCreatableObjectsComboBox->GetSelectIndex();
    if (selectIdx == -1)
    {
        return;
    }
    
    switch ((eCreateObjectType)selectIdx)
    {
    case eCreateObjectType::GameObject:
        CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>(mObjectNameInput->GetTextMultiByte());
        break;
    case eCreateObjectType::Player:
        break;
    default:
        assert(false);
        break;
    }

    CObjectHierachyWindow* hierachyWindow = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Object Hierachy");

    if (hierachyWindow)
    {
        hierachyWindow->AddObjectList(mObjectNameInput->GetTextMultiByte());
    }
}

void CEditorMenuWindow::OnClickCreateComponent()
{
    CObjectHierachyWindow* hierachyWindow = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Object Hierachy");

    CGameObject* obj = nullptr;

    if (hierachyWindow)
    {
        CIMGUIListBox* objList = hierachyWindow->GetObjectListBox();

        if (objList->GetSelectIndex() == -1)
        {
            return;
        }

        obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());
    }

    if (!obj)
    {
        return;
    }

    int selectIdx = mCreatableObjectsComboBox->GetSelectIndex();

    if (selectIdx == -1)
    {
        return;
    }

    switch ((eSceneComponentType)selectIdx)
    {
    case eSceneComponentType::Sprite:
        obj->CreateComponent<CSpriteComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::StaticMesh:
        obj->CreateComponent<CStaticMeshComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    default:
        assert(false);
        break;
    }

    if (hierachyWindow)
    {
        CIMGUIListBox* componentList = hierachyWindow->GetComponentListBox();

        componentList->AddItem(mComponentNameInput->GetTextMultiByte());
    }
}
