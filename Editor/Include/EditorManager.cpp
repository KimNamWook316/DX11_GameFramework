#include "EditorManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/TileMapComponent.h"
#include "Component/ProcedualMapGenerator.h"
#include "Component/DissolveComponent.h"
#include "Component/NavAgentComponent.h"
#include "Component/StateComponent.h"
#include "Diablo2/UI/D2FrameWidget.h"
#include "Diablo2/UI/D2MouseNormal.h"
#include "Diablo2/UI/D2IntroWindow.h"
#include "Diablo2/UI/D2MouseItemSelect.h"
#include "Diablo2/Component/D2CharacterInfoComponent.h"
#include "Diablo2/Component/D2PlayerCollider.h"
#include "Diablo2/Component/D2EnemyCollider.h"
#include "Diablo2/Component/D2DataManagerComponent.h"
#include "Diablo2/Component/D2NavAgentComponent.h"
#include "Diablo2/Component/D2EnemyNavAgentComponent.h"
#include "Diablo2/Component/D2ObjectPoolComponent.h"
#include "Diablo2/Component/D2StateComponent.h"
#include "Diablo2/Component/D2ProjectTile.h"
#include "Diablo2/Component/D2MeleeAttack.h"
#include "Diablo2/Component/D2Blaze.h"
#include "Diablo2/Component/D2FrozenOrb.h"
#include "Diablo2/Component/D2Meteor.h"
#include "Diablo2/Component/D2MeteorFire.h"
#include "Diablo2/Component/D2MeteorTarget.h"
#include "Diablo2/Component/D2Teleport.h"
#include "Diablo2/Component/D2EnemyMeleeAttack.h"
#include "Diablo2/Component/D2PlayerSkillComponent.h"
#include "Diablo2/Component/D2EnemySkillComponent.h"
#include "Diablo2/Component/D2Effect.h"
#include "Diablo2/Component/D2ItemTableComponent.h"
#include "Diablo2/Component/D2UIManagerComponent.h"
#include "Diablo2/Component/D2Inventory.h"
#include "Diablo2/Component/D2ItemObjectComponent.h"
#include "Diablo2/Component/D2ProcedualMapGenerator.h"
#include "Diablo2/Component/D2DungeonManagerComponent.h"
#include "Diablo2/Component/D2ShadowComponent.h"
#include "Diablo2/Component/D2PortalObject.h"
#include "Diablo2/Component/D2ClickableUIComponent.h"
#include "Diablo2/Component/D2ChestObject.h"
#include "Diablo2/State/PlayerIdleState.h"
#include "Diablo2/State/D2EnemyIdleState.h"
#include "Diablo2/State/D2AndarielIdleState.h"
#include "Diablo2/State/D2SkeletonIdleState.h"
#include "Engine.h"
#include "Resource.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Render/RenderManager.h"
#include "Scene/DefaultScene.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Object/SpriteEditObject.h"
#include "Object/DragObject.h"
#include "Window/SpriteWindow.h"
#include "Window/ObjectHierachyWindow.h"
#include "Window/DetailWindow.h"
#include "Window/EditorMenuWindow.h"
#include "Object/Player2D.h"
#include "Object/CameraObject.h"
#include "Component/CameraComponent.h"
#include "Collision/CollisionManager.h"
#include "Diablo2/Resource/Shader/D2ShadowShader.h"
#include "Diablo2/Resource/Mesh/D2ShadowMesh.h"

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager()	:
	meEditMode(eEditMode::Scene),
	mDragObj(nullptr),
	mSpriteWindow(nullptr),
	mbMousePush(false)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		800, 600, IDI_SMALL))
	{
		CEngine::DestroyInst();
		return false;
	}

	CEngine::GetInst()->SetPlay(true);
	CEngine::GetInst()->SetDebugMode(true);

	// Custom Mesh & Shader
	CResourceManager::GetInst()->CreateShader<CD2ShadowShader>("ShadowShader");
	CResourceManager::GetInst()->CreateMesh<CD2ShadowMesh>("ShadowMesh");

	if (!CCollisionManager::GetInst()->LoadProfile("CollsionProfileInfo.csv"))
	{
		assert(false);
		return false;
	}
	
	mCameraObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCameraObject>("CameraObj");
	mCameraObj->ExcludeFromSave(true);

	// SceneManager CallBack
	CSceneManager::GetInst()->SetCreateSceneModeCallBack<CEditorManager>(this, &CEditorManager::OnCreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectCallBack<CEditorManager>(this, &CEditorManager::OnCreateObject);
	CSceneManager::GetInst()->SetCreateComponentCallBack<CEditorManager>(this, &CEditorManager::OnCreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceCallBack<CEditorManager>(this, &CEditorManager::OnCreateAnimInstance);
	CSceneManager::GetInst()->SetCreateStateCallBack<CEditorManager>(this, &CEditorManager::OnCreateState);

	// GUI
	mSpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("Editor");
	mObjectHierachyWindow = CIMGUIManager::GetInst()->AddWindow<CObjectHierachyWindow>("Hierachy");
	mEditorMenuWindow = CIMGUIManager::GetInst()->AddWindow<CEditorMenuWindow>("Editor Menu");
	mDetailWindow = CIMGUIManager::GetInst()->AddWindow<CDetailWindow>("Inspector");

	// Layer
	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	// Key
	CInput::GetInst()->CreateKey("F2", VK_F2);
	CInput::GetInst()->CreateKey("MouseL", VK_LBUTTON);
	CInput::GetInst()->CreateKey("MouseR", VK_RBUTTON);
	CInput::GetInst()->CreateKey("LCtrl", VK_LCONTROL);
	CInput::GetInst()->SetCtrlKey("LCtrl", true);
	CInput::GetInst()->CreateKey("MouseLCtrl", VK_LBUTTON);
	CInput::GetInst()->SetCtrlKey("MouseLCtrl", true);
	CInput::GetInst()->CreateKey("MouseRCtrl", VK_RBUTTON);
	CInput::GetInst()->SetCtrlKey("MouseRCtrl", true);

	// UIKey
	CInput::GetInst()->CreateKey("ToggleInventory", 'I');
	CInput::GetInst()->CreateKey("ToggleSkillTree", 'K');
	CInput::GetInst()->CreateKey("CloseMenu", VK_ESCAPE);
	CInput::GetInst()->CreateKey("ClickableUI", VK_TAB);

	// Skill Debug Key
	CInput::GetInst()->CreateKey("SkillLevelUp", VK_NUMPAD0);
	CInput::GetInst()->CreateKey("NextSkill", VK_NUMPAD1);

	CInput::GetInst()->SetKeyCallBack("F2", eKeyState::KeyState_Down, this, &CEditorManager::OnF2Down);
	CInput::GetInst()->CreateKey("LMouseDrag", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallBack("LMouseDrag", eKeyState::KeyState_Down, this, &CEditorManager::OnMouseLButtonDown);
	CInput::GetInst()->SetKeyCallBack("LMouseDrag", eKeyState::KeyState_Push, this, &CEditorManager::OnMouseLButtonPush);
	CInput::GetInst()->SetKeyCallBack("LMouseDrag", eKeyState::KeyState_Up, this, &CEditorManager::OnMouseLButtonUp);
	
	CInput::GetInst()->CreateKey("DownArrowEditorManager", VK_DOWN);
	CInput::GetInst()->CreateKey("UpArrowEditorManager", VK_UP);
	CInput::GetInst()->CreateKey("LeftArrowEditorManager", VK_LEFT);
	CInput::GetInst()->CreateKey("RightArrowEditorManager", VK_RIGHT);
	CInput::GetInst()->SetKeyCallBack("DownArrowEditorManager", eKeyState::KeyState_Down, this, &CEditorManager::OnDownArrowKeyDown);
	CInput::GetInst()->SetKeyCallBack("UpArrowEditorManager", eKeyState::KeyState_Down, this, &CEditorManager::OnUpArrowKeyDown);
	CInput::GetInst()->SetKeyCallBack("LeftArrowEditorManager", eKeyState::KeyState_Down, this, &CEditorManager::OnLeftArrowKeyDown);
	CInput::GetInst()->SetKeyCallBack("RightArrowEditorManager", eKeyState::KeyState_Down, this, &CEditorManager::OnRightArrowKeyDown);

	CInput::GetInst()->CreateKey("JumpToNextFrameDragObj", VK_RIGHT);
	CInput::GetInst()->CreateKey("JumpToNextFrameReverseDragObj", VK_LEFT);
	CInput::GetInst()->CreateKey("JumpToDownFrameDragObj", VK_DOWN);
	CInput::GetInst()->CreateKey("JumpToUpFrameDragObj", VK_UP);
	CInput::GetInst()->SetShiftKey("JumpToNextFrameReverseDragObj", true);
	CInput::GetInst()->SetShiftKey("JumpToNextFrameDragObj", true);
	CInput::GetInst()->SetShiftKey("JumpToDownFrameDragObj", true);
	CInput::GetInst()->SetShiftKey("JumpToUpFrameDragObj", true);
	CInput::GetInst()->SetKeyCallBack("JumpToNextFrameDragObj", eKeyState::KeyState_Down, this, &CEditorManager::OnShiftRightArrowKeyDown);
	CInput::GetInst()->SetKeyCallBack("JumpToNextFrameReverseDragObj", eKeyState::KeyState_Down, this, &CEditorManager::OnShiftLeftArrowKeyDown);
	CInput::GetInst()->SetKeyCallBack("JumpToDownFrameDragObj", eKeyState::KeyState_Down, this, &CEditorManager::OnShiftDownArrowKeyDown);
	CInput::GetInst()->SetKeyCallBack("JumpToUpFrameDragObj", eKeyState::KeyState_Down, this, &CEditorManager::OnShiftUpArrowKeydown);

	CInput::GetInst()->CreateKey("ScrollDown", 'S');
	CInput::GetInst()->CreateKey("ScrollUp", 'W');
	CInput::GetInst()->CreateKey("ScrollLeft", 'A');
	CInput::GetInst()->CreateKey("ScrollRight", 'D');
	CInput::GetInst()->SetKeyCallBack("ScrollDown", eKeyState::KeyState_Push, this, &CEditorManager::OnScrollDown);
	CInput::GetInst()->SetKeyCallBack("ScrollUp", eKeyState::KeyState_Push, this, &CEditorManager::OnScrollUp);
	CInput::GetInst()->SetKeyCallBack("ScrollLeft", eKeyState::KeyState_Push, this, &CEditorManager::OnScrollLeft);
	CInput::GetInst()->SetKeyCallBack("ScrollRight", eKeyState::KeyState_Push, this, &CEditorManager::OnScrollRight);

	// Drag Object 생성
	mDragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
	mDragObj->SetWorldScale(0.f, 0.f, 1.f);
	mDragObj->Init();
	mDragObj->ExcludeFromSave(true);
	
	std::string outName;
	CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, "DataManager.gobj");
	CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, "ObjectPool.gobj");
	CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, "ItemTable.gobj");
	//CSceneManager::GetInst()->GetScene()->LoadGameObject(outName, "DungeonManager.gobj");
 
	// CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2FrameWidget>("frame");
 
	// CEngine::GetInst()->CreateMouse<CD2MouseNormal>(eMouseState::Normal, "MouseNormal");
	// CEngine::GetInst()->CreateMouse<CD2MouseItemSelect>(eMouseState::State1, "MouseItemSelect");

	CResourceManager::GetInst()->CreateFontFile("Dia2Font", TEXT("Exocet2.ttf"));
	const TCHAR* fontFace = CResourceManager::GetInst()->GetFontFaceName("Dia2Font");
	CResourceManager::GetInst()->LoadFont("Dia2Font", fontFace, 300, 20.f, TEXT("eng"));

	//CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2IntroWindow>("Intro");

	return true;
}

void CEditorManager::Start()
{
	CGameObject* player = CSceneManager::GetInst()->GetScene()->FindObject("Player");
	CSceneManager::GetInst()->GetScene()->SetPlayerObj(player);
}

void CEditorManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CDefaultScene>();
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

// 씬 로드될 때 씬에서 Obj List Clear 해버리기 떄문에 다시 생성한다.
void CEditorManager::RecreateEditorCameraDragObj()
{
	mCameraObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCameraObject>("CameraObj");
	mCameraObj->ExcludeFromSave(true);

	mDragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
	mDragObj->SetWorldScale(0.f, 0.f, 1.f);
	mDragObj->Init();
	mDragObj->ExcludeFromSave(true);
}

void CEditorManager::OnCreateSceneMode(CScene* scene, size_t type)
{
	if (type == typeid(CDefaultScene).hash_code())
	{
		scene->LoadSceneMode<CDefaultScene>();
	}
}

class CGameObject* CEditorManager::OnCreateObject(CScene* scene, size_t type)
{
	if (type == typeid(CGameObject).hash_code())
	{
		CGameObject* obj = scene->CreateEmptyObjectByType<CGameObject>();
		return obj;
	}
	else if (type == typeid(CCameraObject).hash_code())
	{
		CCameraObject* obj = scene->CreateEmptyObjectByType<CCameraObject>();
		return obj;
	}
	else if (type == typeid(CDragObject).hash_code())
	{
		CDragObject* obj = scene->CreateEmptyObjectByType<CDragObject>();
		return obj;
	}
	else if (type == typeid(CSpriteEditObject).hash_code())
	{
		CSpriteEditObject* obj = scene->CreateEmptyObjectByType<CSpriteEditObject>();
		return obj;
	}
	else if (type == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* obj = scene->CreateEmptyObjectByType<CPlayer2D>();
		return obj;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

CComponent* CEditorManager::OnCreateComponent(CGameObject* obj, size_t type)
{
	if (type == typeid(CSceneComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSceneComponent>();
		return component;
	}
	else if (type == typeid(CSpriteComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSpriteComponent>();
		return component;
	}
	else if(type == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CStaticMeshComponent>();
		return component;
	}
	else if(type == typeid(CColliderBox2D).hash_code())
	{
		CComponent* component = obj->LoadComponent<CColliderBox2D>();
		return component;
	}
	else if(type == typeid(CColliderCircle).hash_code())
	{
		CComponent* component = obj->LoadComponent<CColliderCircle>();
		return component;
	}
	else if(type == typeid(CColliderPixel).hash_code())
	{
		CComponent* component = obj->LoadComponent<CColliderPixel>();
		return component;
	}
	else if(type == typeid(CCameraComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CCameraComponent>();
		return component;
	}
	else if(type == typeid(CWidgetComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CWidgetComponent>();
		return component;
	}
	else if(type == typeid(CParticleComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CParticleComponent>();
		return component;
	}
	else if(type == typeid(CTileMapComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CTileMapComponent>();
		return component;
	}
	else if(type == typeid(CProcedualMapGenerator).hash_code())
	{
		CComponent* component = obj->LoadComponent<CProcedualMapGenerator>();
		return component;
	}
	else if(type == typeid(CDissolveComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CDissolveComponent>();
		return component;
	}
	else if(type == typeid(CNavAgentComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CNavAgentComponent>();
		return component;
	}
	else if(type == typeid(CStateComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CStateComponent>();
		return component;
	}
	else if(type == typeid(CD2CharacterInfoComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2CharacterInfoComponent>();
		return component;
	}
	else if(type == typeid(CD2DataManagerComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2DataManagerComponent>();
		return component;
	}
	else if(type == typeid(CD2NavAgentComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2NavAgentComponent>();
		return component;
	}
	else if(type == typeid(CD2EnemyNavAgentComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemyNavAgentComponent>();
		return component;
	}
	else if(type == typeid(CD2ObjectPoolComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ObjectPoolComponent>();
		return component;
	}
	else if(type == typeid(CD2Projectile).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Projectile>();
		return component;
	}
	else if(type == typeid(CD2MeleeAttack).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2MeleeAttack>();
		return component;
	}
	else if(type == typeid(CD2PlayerSkillComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2PlayerSkillComponent>();
		return component;
	}
	else if(type == typeid(CD2Blaze).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Blaze>();
		return component;
	}
	else if(type == typeid(CD2FrozenOrb).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2FrozenOrb>();
		return component;
	}
	else if(type == typeid(CD2Meteor).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Meteor>();
		return component;
	}
	else if(type == typeid(CD2MeteorFire).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2MeteorFire>();
		return component;
	}
	else if(type == typeid(CD2MeteorTarget).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2MeteorTarget>();
		return component;
	}
	else if(type == typeid(CD2Teleport).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Teleport>();
		return component;
	}
	else if(type == typeid(CD2StateComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2StateComponent>();
		return component;
	}
	else if(type == typeid(CD2PlayerCollider).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2PlayerCollider>();
		return component;
	}
	else if(type == typeid(CD2EnemyCollider).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemyCollider>();
		return component;
	}
	else if(type == typeid(CD2EnemySkillComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemySkillComponent>();
		return component;
	}
	else if(type == typeid(CD2EnemyMeleeAttack).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2EnemyMeleeAttack>();
		return component;
	}
	else if(type == typeid(CD2Effect).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Effect>();
		return component;
	}
	else if(type == typeid(CD2UIManagerComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2UIManagerComponent>();
		return component;
	}
	else if(type == typeid(CD2ItemTableComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ItemTableComponent>();
		return component;
	}
	else if(type == typeid(CD2Inventory).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2Inventory>();
		return component;
	}
	else if(type == typeid(CD2ItemObjectComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ItemObjectComponent>();
		return component;
	}
	else if(type == typeid(CD2ProcedualMapGenerator).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ProcedualMapGenerator>();
		return component;
	}
	else if(type == typeid(CD2DungeonManagerComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2DungeonManagerComponent>();
		return component;
	}
	else if(type == typeid(CD2ShadowComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ShadowComponent>();
		return component;
	}
	else if(type == typeid(CD2PortalObject).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2PortalObject>();
		return component;
	}
	else if(type == typeid(CD2ClickableUIComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ClickableUIComponent>();
		return component;
	}
	else if(type == typeid(CD2ChestObject).hash_code())
	{
		CComponent* component = obj->LoadComponent<CD2ChestObject>();
		return component;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

void CEditorManager::OnCreateAnimInstance(CSpriteComponent* comp, size_t type)
{
	if (type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		comp->SceneLoadAnimationInstance<CAnimationSequence2DInstance>();
	}
}

void CEditorManager::OnCreateState(CStateComponent* comp, size_t type)
{
	if (type == typeid(CPlayerIdleState).hash_code())
	{
		comp->SetInitialState<CPlayerIdleState>();
	}
	else if (type == typeid(CD2EnemyIdleState).hash_code())
	{
		comp->SetInitialState<CD2EnemyIdleState>();
	}
	else if (type == typeid(CD2AndarielIdleState).hash_code())
	{
		comp->SetInitialState<CD2AndarielIdleState>();
	}
	else if (type == typeid(CD2SkeletonIdleState).hash_code())
	{
		comp->SetInitialState<CD2SkeletonIdleState>();
	}
}

void CEditorManager::OnMouseLButtonDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();
		mDragObj->SetStartPos(mousePos);

		mSpriteWindow->SetCropStartPos(mousePos);
	}
}

void CEditorManager::OnMouseLButtonPush(float deltaTime)
{
	mbMousePush = true;

	if (eEditMode::Sprite == meEditMode)
	{
		mDragObj->SetEndPos(CInput::GetInst()->GetMouseWorld2DPos());
	}
}

void CEditorManager::OnMouseLButtonUp(float deltaTime)
{
	mbMousePush = false;

	if (eEditMode::Sprite == meEditMode)
	{
		mSpriteWindow->SetCropEndPos(CInput::GetInst()->GetMouseWorld2DPos());
		mSpriteWindow->UpdateCropImage();
	}
}

void CEditorManager::OnDownArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		mDragObj->AddWorldPos(0.f,-1.f, 0.f);
		mSpriteWindow->MoveCropPos(0.f, -1.f);
	}
}

void CEditorManager::OnUpArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		mDragObj->AddWorldPos(0.f, 1.f, 0.f);
		mSpriteWindow->MoveCropPos(0.f, 1.f);
	}
}

void CEditorManager::OnLeftArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		mDragObj->AddWorldPos(-1.f, 0.f, 0.f);
		mSpriteWindow->MoveCropPos(-1.f, 0.f);
	}
}

void CEditorManager::OnRightArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		mDragObj->AddWorldPos(1.f, 0.f, 0.f);
		mSpriteWindow->MoveCropPos(1.f, 0.f);
	}
}

void CEditorManager::OnShiftRightArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		float width = abs(mDragObj->GetRelativeScale().x);
		mDragObj->AddWorldPos(width, 0.f, 0.f);

		mSpriteWindow->MoveCropPos(width, 0);
	}
}

void CEditorManager::OnShiftLeftArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		float width = abs(mDragObj->GetRelativeScale().x);
		mDragObj->AddWorldPos(-width, 0.f, 0.f);

		mSpriteWindow->MoveCropPos(-width, 0);
	}
}

void CEditorManager::OnShiftDownArrowKeyDown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		float height = abs(mDragObj->GetRelativeScale().y);
		mDragObj->AddWorldPos(0.f, -height, 0.f);

		mSpriteWindow->MoveCropPos(0.f, -height);
	}
}

void CEditorManager::OnShiftUpArrowKeydown(float deltaTime)
{
	if (eEditMode::Sprite == meEditMode)
	{
		float height = abs(mDragObj->GetRelativeScale().y);
		mDragObj->AddWorldPos(0.f, height, 0.f);

		mSpriteWindow->MoveCropPos(0.f, height);
	}
}

void CEditorManager::OnScrollDown(float deltaTime)
{
	if (mCameraObj)
	{
		mCameraObj->AddWorldPos(0.f, -deltaTime * 500.f, 0.f);
	}
}

void CEditorManager::OnScrollUp(float deltaTime)
{
	if (mCameraObj)
	{
		mCameraObj->AddWorldPos(0.f, deltaTime * 500.f, 0.f);
	}
}

void CEditorManager::OnScrollLeft(float deltaTime)
{
	if (mCameraObj)
	{
		mCameraObj->AddWorldPos(-deltaTime * 500.f, 0.f, 0.f);
	}
}

void CEditorManager::OnScrollRight(float deltaTime)
{
	if (mCameraObj)
	{
		mCameraObj->AddWorldPos(deltaTime * 500.f, 0.f, 0.f);
	}
}

void CEditorManager::OnF2Down(float deltaTime)
{
	if (CEngine::GetInst()->IsDebugMode())
	{
		CEngine::GetInst()->SetDebugMode(false);
	}
	else
	{
		CEngine::GetInst()->SetDebugMode(true);
	}
}

void CEditorManager::SetEditMode(eEditMode mode)
{
	meEditMode = mode;

	switch (meEditMode)
	{
	case eEditMode::Scene:
		break;
	case eEditMode::Sprite:
		break;
	case eEditMode::TileMap:
		if (mDragObj)
		{
			mDragObj->Destroy();
			mDragObj = nullptr;
		}
		break;
	default:
		assert(false);
		break;
	}
}
