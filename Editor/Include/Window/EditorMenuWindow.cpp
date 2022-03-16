#include "EditorMenuWindow.h"
#include "../EditorManager.h"
#include "DetailWindow.h"
#include "IMGUIButton.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIText.h"
#include "IMGUISeperator.h"
#include "IMGUIListBox.h"
#include "IMGUITree.h"
#include "IMGUISameLine.h"
#include "IMGUIMainMenuBar.h"
#include "IMGUIBeginMenu.h"
#include "IMGUIMenuItem.h"
#include "../EditorInfo.h"
#include "Scene/SceneManager.h"
#include "ObjectHierachyWindow.h"
#include "TileSetEditWindow.h"
#include "CollisionProfileWindow.h"
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
#include "Component/StateComponent.h"
#include "../Diablo2/Component/D2CharacterInfoComponent.h"
#include "../Diablo2/Component/D2PlayerCollider.h"
#include "../Diablo2/Component/D2EnemyCollider.h"
#include "../Diablo2/Component/D2StateComponent.h"
#include "../Diablo2/Component/D2DataManagerComponent.h"
#include "../Diablo2/Component/D2NavAgentComponent.h"
#include "../Diablo2/Component/D2EnemyNavAgentComponent.h"
#include "../Diablo2/Component/D2ObjectPoolComponent.h"
#include "../Diablo2/Component/D2ProjectTile.h"
#include "../Diablo2/Component/D2MeleeAttack.h"
#include "../Diablo2/Component/D2EnemyMeleeAttack.h"
#include "../Diablo2/Component/D2PlayerSkillComponent.h"
#include "../Diablo2/Component/D2EnemySkillComponent.h"
#include "../Diablo2/Component/D2Blaze.h"
#include "../Diablo2/Component/D2FrozenOrb.h"
#include "../Diablo2/Component/D2Meteor.h"
#include "../Diablo2/Component/D2MeteorFire.h"
#include "../Diablo2/Component/D2MeteorTarget.h"
#include "../Diablo2/Component/D2Teleport.h"
#include "../Diablo2/Component/D2PlayerSkillComponent.h"
#include "../Diablo2/State/PlayerIdleState.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"

// D2
#include "../Diablo2/D2Info.h"
#include "../Diablo2/D2Util.h"

CEditorMenuWindow::CEditorMenuWindow()  :
    mCreateObjectButton(nullptr),
    mCreateComponentButton(nullptr),
    mObjectNameInput(nullptr),
    mComponentNameInput(nullptr),
    mCreatableObjectsComboBox(nullptr),
    mCreatableComponentsComboBox(nullptr),
    mMainMenuBar(nullptr)
{
}

CEditorMenuWindow::~CEditorMenuWindow()
{
}

bool CEditorMenuWindow::Init()
{
    CIMGUIWindow::Init();

    mMainMenuBar = AddWidget<CIMGUIMainMenuBar>("Main Menu");
    CIMGUIBeginMenu* beginMenu = mMainMenuBar->AddWidget<CIMGUIBeginMenu>("File");
    CIMGUIMenuItem* item = beginMenu->AddWidget<CIMGUIMenuItem>("Save Scene");
    item->SetClickCallBack(this, &CEditorMenuWindow::OnClickSaveScene);
    item = beginMenu->AddWidget<CIMGUIMenuItem>("Load Scene");
    item->SetClickCallBack(this, &CEditorMenuWindow::OnClickLoadScene);
    item = beginMenu->AddWidget<CIMGUIMenuItem>("Save Object");
    item->SetClickCallBack(this, &CEditorMenuWindow::OnClickSaveObject);
    item = beginMenu->AddWidget<CIMGUIMenuItem>("Load Object");
    item->SetClickCallBack(this, &CEditorMenuWindow::OnClickLoadObject);

    beginMenu = mMainMenuBar->AddWidget<CIMGUIBeginMenu>("Edit");
    item = beginMenu->AddWidget<CIMGUIMenuItem>("Sprite Editor");
    mOpenCollsionProfileMenuButton = beginMenu->AddWidget<CIMGUIMenuItem>("Collision Profile Editor");
    mOpenCollsionProfileMenuButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickOpenCollisionProfileEditor);
    mOpenTileSetEditorMenuButton = beginMenu->AddWidget<CIMGUIMenuItem>("TileSet Editor");
    mOpenTileSetEditorMenuButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickOpenTileSetEditor);

    CIMGUIText* text = AddWidget<CIMGUIText>("text");
    text->SetText("Play/Stop");

    CIMGUIButton* button = AddWidget<CIMGUIButton>("Play", 0.f, 0.f);
    button->SetClickCallBack(this, &CEditorMenuWindow::OnClickPlay);

    AddWidget<CIMGUISeperator>("seperator");

    // Create Object Tree
    CIMGUITree* tree = AddWidget<CIMGUITree>("Create Object");
    mCreatableObjectsComboBox = tree->AddWidget<CIMGUIComboBox>("Object List", 150.f, 0.f);
    mCreatableObjectsComboBox->AddItem("GameObject");
    mCreatableObjectsComboBox->AddItem("Player");
    mObjectNameInput = tree->AddWidget<CIMGUITextInput>("Object Name", 100.f, 0.f);
    mCreateObjectButton = tree->AddWidget<CIMGUIButton>("Create Object", 0.f, 0.f);
    mCreateObjectButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickCreateObject);
    AddWidget<CIMGUISeperator>("seperator");

    // Create Component Tree
    tree = AddWidget<CIMGUITree>("Create Component");
    mCreatableComponentsComboBox = tree->AddWidget<CIMGUIComboBox>("Component List", 150.f, 0.f);

    for (int i = 0; i < (int)eSceneComponentType::Max; ++i)
    {
        mCreatableComponentsComboBox->AddItem(CUtil::SceneComponentTypeToString((eSceneComponentType)i));
    }
    for (int i = 0; i < (int)eObjectComponentType::Max; ++i)
    {
        mCreatableComponentsComboBox->AddItem(CUtil::ObjectComponentTypeToString((eObjectComponentType)i));
    }
    for (int i = 0; i < (int)eD2SceneComponentType::Max; ++i)
    {
        mCreatableComponentsComboBox->AddItem(CD2Util::D2SceneComponentTypeToString((eD2SceneComponentType)i));
    }
    for (int i = 0; i < (int)eD2ObjectComponentType::Max; ++i)
    {
        mCreatableComponentsComboBox->AddItem(CD2Util::D2ObjectComponentTypeToString((eD2ObjectComponentType)i));
    }
    mComponentNameInput = tree->AddWidget<CIMGUITextInput>("Component Name", 100.f, 0.f);
    mCreateComponentButton = tree->AddWidget<CIMGUIButton>("Create Component", 0.f, 0.f);
    mCreateComponentButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickCreateComponent);
    AddWidget<CIMGUISeperator>("seperator");

    // Prefab Tree
    tree = AddWidget<CIMGUITree>("Prefab");
    text = tree->AddWidget<CIMGUIText>("text");
    text->SetText("Prefab List");
    mCreatablePrefabsListBox = tree->AddWidget<CIMGUIListBox>("Prefabs");
    mCreatablePrefabsListBox->SetHideName(true);
    mCreatablePrefabsListBox->SetSize(200.f, 100.f);
    mCreatablePrefabsListBox->SetPageItemCount(10);
    mInstanciatePrefabButton = tree->AddWidget<CIMGUIButton>("Instantiate", 0.f, 0.f);
    mInstanciatePrefabButton->SetClickCallBack(this, &CEditorMenuWindow::OnClickInstanciate);
    refreshPrefabList();

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
    case eSceneComponentType::Scene:
        sceneComp = obj->CreateComponentAddChild<CSceneComponent>(mComponentNameInput->GetTextMultiByte());
        break;
    case eSceneComponentType::Sprite:
        sceneComp = obj->CreateComponentAddChild<CSpriteComponent>(mComponentNameInput->GetTextMultiByte());
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
        case eObjectComponentType::State:
            objComp = obj->CreateComponent<CStateComponent>(mComponentNameInput->GetTextMultiByte());
            break;
		}

    }
    
    // 사용자 정의 컴포넌트
    if (!objComp)
    {
        selectIdx -= (int)(eObjectComponentType::Max);

        switch ((eD2SceneComponentType)selectIdx)
        {
        }
    }

    if (!sceneComp && !objComp)
    {
        selectIdx -= (int)(eD2SceneComponentType::Max);

        switch ((eD2ObjectComponentType)selectIdx)
        {
        case eD2ObjectComponentType::D2PlayerCollider:
            objComp = obj->CreateComponent<CD2PlayerCollider>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2EnemyCollider:
            objComp = obj->CreateComponent<CD2EnemyCollider>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2CharacterInfo:
            objComp = obj->CreateComponent<CD2CharacterInfoComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2State:
            objComp = obj->CreateComponent<CD2StateComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2DataManager:
            objComp = obj->CreateComponent<CD2DataManagerComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2NavAgent:
            objComp = obj->CreateComponent<CD2NavAgentComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2EnemyNavAgent:
            objComp = obj->CreateComponent<CD2EnemyNavAgentComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2ObjectPool:
            objComp = obj->CreateComponent<CD2ObjectPoolComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2Projectile:
            objComp = obj->CreateComponent<CD2Projectile>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2MeleeAttack:
            objComp = obj->CreateComponent<CD2MeleeAttack>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2PlayerSkill:
            objComp = obj->CreateComponent<CD2PlayerSkillComponent>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2EnemySkill:
            objComp = obj->CreateComponent<CD2EnemySkillComponent>(mComponentNameInput->GetTextMultiByte());
            break;
        case eD2ObjectComponentType::D2Blaze:
            objComp = obj->CreateComponent<CD2Blaze>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2FrozenOrb:
            objComp = obj->CreateComponent<CD2FrozenOrb>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2Meteor:
            objComp = obj->CreateComponent<CD2Meteor>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2MeteorFire:
            objComp = obj->CreateComponent<CD2MeteorFire>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2MeteorTarget:
            objComp = obj->CreateComponent<CD2MeteorTarget>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2Teleport:
            objComp = obj->CreateComponent<CD2Teleport>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        case eD2ObjectComponentType::D2EnemyMeleeAttack:
            objComp = obj->CreateComponent<CD2EnemyMeleeAttack>(mComponentNameInput->GetTextMultiByte());
            static_cast<CD2SkillObject*>(objComp)->SetEditMode(true);
            break;
        }
    }

    // 예외처리
    if (!sceneComp && !objComp)
    {
        assert(false);
        return;
    }
    
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

            CObjectHierachyWindow* hierachyWindow = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(HIERACHY_WINDOW_NAME);
            
            size_t size = objNames.size();
            for (size_t i = 0; i < size; ++i)
            {
                hierachyWindow->AddObjectList(objNames[i].c_str());
            }

            CEditorManager::GetInst()->RecreateEditorCameraDragObj();

            MessageBox(nullptr, TEXT("씬 로드 성공"), TEXT("Failed"), MB_OK);
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
        refreshPrefabList();
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

void CEditorMenuWindow::OnClickInstanciate()
{
    if (-1 == mCreatablePrefabsListBox->GetSelectIndex())
    {
        return;
    }

    std::string outName;
    std::string fileName = mCreatablePrefabsListBox->GetSelectItem();
    CGameObject* obj = CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, fileName.c_str());

    // Script의 경우 이 버튼을 통해 생성할 경우 에디터 모드로 전환하게
    CD2SkillObject* skillScript = static_cast<CD2SkillObject*>(obj->FindComponent("Script"));
    if (skillScript)
    {
        skillScript->SetEditMode(true);
    }
}

void CEditorMenuWindow::OnClickOpenCollisionProfileEditor()
{
	if (!CIMGUIManager::GetInst()->FindIMGUIWindow(COLLSIONPROFILE_WINDOW_NAME))
	{
		CIMGUIManager::GetInst()->AddWindow<CCollisionProfileWindow>(COLLSIONPROFILE_WINDOW_NAME);
	}
}

void CEditorMenuWindow::OnClickOpenTileSetEditor()
{
	if (!CIMGUIManager::GetInst()->FindIMGUIWindow(TILESETEDITOR_WINDOW_NAME))
	{
		CIMGUIManager::GetInst()->AddWindow<CTileSetEditWindow>(TILESETEDITOR_WINDOW_NAME);
	}
}

void CEditorMenuWindow::refreshPrefabList()
{
    mCreatablePrefabsListBox->Clear();

    const PathInfo* path = CPathManager::GetInst()->FindPath(OBJECT_PATH);
    std::vector<std::string> prefabNames = CEditorUtil::GetFilesInDirectory(path->PathMultibyte, "*.gobj");
    size_t size = prefabNames.size();
    for (size_t i = 0; i < size; ++i)
    {
        mCreatablePrefabsListBox->AddItem(prefabNames[i]);
    }
}
