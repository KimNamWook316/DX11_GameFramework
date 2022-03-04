#include "EditorMenuWindow.h"
#include "../EditorManager.h"
#include "DetailWindow.h"
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
#include "Component/TileMapComponent.h"
#include "Component/DissolveComponent.h"
#include "Component/NavAgentComponent.h"
#include "Component/ProcedualMapGenerator.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "../EditorInfo.h"

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

    for (int i = 0; i < (int)eSceneComponentType::Max; ++i)
    {
        mCreatableComponentsComboBox->AddItem(CUtil::SceneComponentTypeToString((eSceneComponentType)i));
    }
    for (int i = 0; i < (int)eObjectComponentType::Max; ++i)
    {
        mCreatableComponentsComboBox->AddItem(CUtil::ObjectComponentTypeToString((eObjectComponentType)i));
    }

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

    AddWidget<CIMGUISeperator>("seperator");
    text = AddWidget<CIMGUIText>("text");
    text->SetText("Object Save/Load");
    mSaveObjectButton = AddWidget<CIMGUIButton>("Save GameObject", 0.f, 0.f);
    mSaveObjectButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickSaveObject);
    AddWidget<CIMGUISameLine>("Line");
    mLoadObjectButton = AddWidget<CIMGUIButton>("Load GameObject", 0.f, 0.f);
    mLoadObjectButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickLoadObject);

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

    CObjectHierachyWindow* hierachyWindow = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Hierachy");

    if (hierachyWindow)
    {
        hierachyWindow->AddObjectList(mObjectNameInput->GetTextMultiByte());
    }
}

void CEditorMenuWindow::OnClickCreateComponent()
{
    CObjectHierachyWindow* hierachyWindow = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Hierachy");

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

    CSceneComponent* sceneComp = nullptr;

    // 엔진 내장 컴포넌트들
    switch ((eSceneComponentType)selectIdx)
    {
    case eSceneComponentType::Sprite:
        sceneComp = obj->CreateComponent<CSpriteComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::StaticMesh:
        sceneComp = obj->CreateComponentAddChild<CStaticMeshComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::ColliderBox2D:
        sceneComp = obj->CreateComponentAddChild<CColliderBox2D>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::ColliderCircle:
        sceneComp = obj->CreateComponentAddChild<CColliderCircle>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::ColliderPixel:
        sceneComp = obj->CreateComponentAddChild<CColliderPixel>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::Camera:
        sceneComp = obj->CreateComponentAddChild<CCameraComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::Widget:
        sceneComp = obj->CreateComponentAddChild<CWidgetComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::Particle:
        sceneComp = obj->CreateComponentAddChild<CParticleComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::TileMap:
    {
        sceneComp = obj->CreateComponentAddChild<CTileMapComponent>(mComponentNameInput->GetTextMultiByte());

        CEditorManager::GetInst()->SetEditMode(eEditMode::TileMap);
        break;
    }
    case eSceneComponentType::ProcedualMapGenerator:
    {
        sceneComp = obj->CreateComponentAddChild<CProcedualMapGenerator>(mComponentNameInput->GetTextMultiByte());
        break;
    }
    }

	CObjectComponent* objComp = nullptr;

    if (!sceneComp)
    {
		selectIdx -= (int)(eSceneComponentType::Max);

		switch ((eObjectComponentType)selectIdx)
		{
		case eObjectComponentType::Dissolve:
			objComp = obj->CreateComponent<CDissolveComponent>(mComponentNameInput->GetTextMultiByte());
			break;
		case eObjectComponentType::NavAgent:
			objComp = obj->CreateComponent<CNavAgentComponent>(mComponentNameInput->GetTextMultiByte());
			break;
		}
    }
    

    // TODO : 사용자 정의 컴포넌트에도 없으면 assert 하도록
    
    if (sceneComp)
    {
        sceneComp->Start();
    }
    else if (objComp)
    {
        objComp->Start();
    }

    // hierachy component list에 추가
    if (hierachyWindow)
    {
        CIMGUIListBox* componentList = hierachyWindow->GetComponentListBox();
        componentList->AddItem(mComponentNameInput->GetTextMultiByte());
    }

    // inspector update
    CDetailWindow* inspector = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Inspector");
    inspector->UpdateComponentUI(obj);
}

void CEditorMenuWindow::OnClickSaveScene()
{
    TCHAR filePath[MAX_PATH] = {};

    OPENFILENAME openFile = {};

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    openFile.lpstrFilter = TEXT("모든 파일\0*.*\0Scene File\0*.scn");
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
    openFile.lpstrFilter = TEXT("모든 파일\0*.*\0Scene File\0*.scn");
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

void CEditorMenuWindow::OnClickSaveObject()
{
    CObjectHierachyWindow* hierachy = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(HIERACHY_WINDOW_NAME);

    if (-1 == hierachy->GetObjectListBox()->GetSelectIndex())
    {
        return;
    }

    std::string objName = hierachy->GetObjectListBox()->GetSelectItem();

    TCHAR filePath[MAX_PATH] = {};

    OPENFILENAME openFile = {};

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    openFile.lpstrFilter = TEXT("GameObject File\0*.gobj\0All Files\0*.*");
    openFile.lpstrFile = filePath;
    openFile.nMaxFile = MAX_PATH;
    openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

    if (GetSaveFileName(&openFile) != 0)
    {
        char convertFullPath[MAX_PATH] = {};

        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

        if (!CSceneManager::GetInst()->GetScene()->SaveGameObjectFullPath(objName, convertFullPath))
        {
			MessageBox(nullptr, TEXT("게임오브젝트 저장 실패"), TEXT("Failed"), MB_OK);
        }

        MessageBox(nullptr, TEXT("게임오브젝트 저장 완료"), TEXT("Succes"), MB_OK);
    }
}

void CEditorMenuWindow::OnClickLoadObject()
{ 
    TCHAR filePath[MAX_PATH] = {};

    OPENFILENAME openFile = {};

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    openFile.lpstrFilter = TEXT("GameObject File\0*.gobj\0All Files\0*.*");
    openFile.lpstrFile = filePath;
    openFile.nMaxFile = MAX_PATH;
    openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

    if (GetOpenFileName(&openFile) != 0)
    {
        char convertFullPath[MAX_PATH] = {};

        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

        std::string outName;
        CSceneManager::GetInst()->GetScene()->LoadGameObjectFullPath(outName, convertFullPath);

		CObjectHierachyWindow* hierachy = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(HIERACHY_WINDOW_NAME);
        hierachy->GetObjectListBox()->AddItem(outName);
    }
}
