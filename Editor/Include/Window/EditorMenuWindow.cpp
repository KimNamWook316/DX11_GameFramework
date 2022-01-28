#include "EditorMenuWindow.h"
#include "IMGUIButton.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUISeperator.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "../EditorInfo.h"
#include "Scene/SceneManager.h"
#include "ObjectHierachyWindow.h"
#include "SpriteWindow.h"
#include "../Object/Player2D.h"
#include "IMGUIManager.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ParticleComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"

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
    text->SetText("Play/Stop");

    CIMGUIButton* button = AddWidget<CIMGUIButton>("Play", 0.f, 0.f);
    button->SetClickCallBack(this, &CEditorMenuWindow::OnClickPlay);

    AddWidget<CIMGUISeperator>("seperator");

    text = AddWidget<CIMGUIText>("text");
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
    mCreatableComponentsComboBox->AddItem("ColliderBox2D");
    mCreatableComponentsComboBox->AddItem("ColliderCircle");
    mCreatableComponentsComboBox->AddItem("ColliderPixel");
    mCreatableComponentsComboBox->AddItem("Camera");
    mCreatableComponentsComboBox->AddItem("Widget");
    mCreatableComponentsComboBox->AddItem("Particle");

    mComponentNameInput = AddWidget<CIMGUITextInput>("Component Name", 100.f, 0.f);
    
    mCreateComponentButton = AddWidget<CIMGUIButton>("Create Component", 0.f, 0.f);
    mCreateComponentButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickCreateComponent);
    
    AddWidget<CIMGUISeperator>("seperator");

    text = AddWidget<CIMGUIText>("text");
    text->SetText("Scene Save/Load");

    button = AddWidget<CIMGUIButton>("Save Scene", 0.f, 0.f);
    button->SetClickCallBack(this, &CEditorMenuWindow::OnClickSaveScene);

    AddWidget<CIMGUISameLine>("Line");

    button = AddWidget<CIMGUIButton>("Load Scene", 0.f, 0.f);
    button->SetClickCallBack(this, &CEditorMenuWindow::OnClickLoadScene);
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
        CSceneManager::GetInst()->GetScene()->CreateGameObject<CPlayer2D>(mObjectNameInput->GetTextMultiByte());
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

    int selectIdx = mCreatableComponentsComboBox->GetSelectIndex();

    if (selectIdx == -1)
    {
        return;
    }

    switch ((eCreateSceneComponentType)selectIdx)
    {
    case eCreateSceneComponentType::Sprite:
        obj->CreateComponent<CSpriteComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::StaticMesh:
        obj->CreateComponentAddChild<CStaticMeshComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::ColliderBox2D:
        obj->CreateComponentAddChild<CColliderBox2D>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::ColliderCircle:
        obj->CreateComponentAddChild<CColliderCircle>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::ColliderPixel:
        obj->CreateComponentAddChild<CColliderPixel>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::Camera:
        obj->CreateComponentAddChild<CCameraComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::Widget:
        obj->CreateComponentAddChild<CWidgetComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eCreateSceneComponentType::Particle:
        obj->CreateComponentAddChild<CParticleComponent>(mComponentNameInput->GetTextMultiByte());
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

void CEditorMenuWindow::OnClickSaveScene()
{
    TCHAR filePath[MAX_PATH] = {};

    OPENFILENAME openFile = {};

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    openFile.lpstrFilter = TEXT("葛电 颇老\0*.*\0Scene File\0*.scn");
    openFile.lpstrFile = filePath;
    openFile.nMaxFile = MAX_PATH;
    openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

    if (GetSaveFileName(&openFile) != 0)
    {
        char convertFullPath[MAX_PATH] = {};

        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

        CSceneManager::GetInst()->GetScene()->SaveFullPath(convertFullPath);
    }
}

void CEditorMenuWindow::OnClickLoadScene()
{
    TCHAR filePath[MAX_PATH] = {};

    OPENFILENAME openFile = {};

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    openFile.lpstrFilter = TEXT("葛电 颇老\0*.*\0Scene File\0*.scn");
    openFile.lpstrFile = filePath;
    openFile.nMaxFile = MAX_PATH;
    openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

    if (GetOpenFileName(&openFile) != 0)
    {
        char convertFullPath[MAX_PATH] = {};

        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

        if (!CSceneManager::GetInst()->GetScene()->LoadFullPath(convertFullPath))
        {
            MessageBox(nullptr, TEXT("Failed To Load Scene"), TEXT("Failed"), MB_OK);
        }
        else
        {
            std::vector<std::string> objNames;
            CSceneManager::GetInst()->GetScene()->GetObjNames(objNames);

            CObjectHierachyWindow* hierachyWindow = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Object Hierachy");
            
            size_t size = objNames.size();
            for (size_t i = 0; i < size; ++i)
            {
                hierachyWindow->AddObjectList(objNames[i].c_str());
            }
        }
    }
}

void CEditorMenuWindow::OnClickPlay()
{
    if (!CEngine::GetInst()->IsPlay())
    {
        CEngine::GetInst()->SetPlay(true);
    }
    else
    {
        CEngine::GetInst()->SetPlay(false);
    }
}
