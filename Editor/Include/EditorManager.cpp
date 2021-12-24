#include "EditorManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
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

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager()	:
	meEditMode(eEditMode::Scene),
	mDragObj(nullptr),
	mSpriteWindow(nullptr)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		1280, 720, IDI_SMALL))
	{
		CEngine::DestroyInst();
		return false;
	}

	CEngine::GetInst()->SetPlay(false);
	
	// SceneManager CallBack
	CSceneManager::GetInst()->SetCreateSceneModeCallBack<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectCallBack<CEditorManager>(this, &CEditorManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentCallBack<CEditorManager>(this, &CEditorManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceCallBack<CEditorManager>(this, &CEditorManager::CreateAnimInstance);

	// GUI
	mSpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("Editor");
	mObjectHierachyWindow = CIMGUIManager::GetInst()->AddWindow<CObjectHierachyWindow>("Object Hierachy");
	mDetailWindow = CIMGUIManager::GetInst()->AddWindow<CDetailWindow>("Details");
	mEditorMenuWindow = CIMGUIManager::GetInst()->AddWindow<CEditorMenuWindow>("Editor Menu");

	// Layer
	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	// Key
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

	return true;
}

void CEditorManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CDefaultScene>();
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CEditorManager::CreateSceneMode(CScene* scene, size_t type)
{
	if (type == typeid(CDefaultScene).hash_code())
	{
		scene->LoadSceneMode<CDefaultScene>();
	}
}

class CGameObject* CEditorManager::CreateObject(CScene* scene, size_t type)
{
	if (type == typeid(CGameObject).hash_code())
	{
		CGameObject* obj = scene->LoadGameObject<CGameObject>();
		return obj;
	}
	else if (type == typeid(CDragObject).hash_code())
	{
		CDragObject* obj = scene->LoadGameObject<CDragObject>();
		return obj;
	}
	else if (type == typeid(CSpriteEditObject).hash_code())
	{
		CSpriteEditObject* obj = scene->LoadGameObject<CSpriteEditObject>();
		return obj;
	}
	else if (type == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* obj = scene->LoadGameObject<CPlayer2D>();
		return obj;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

CComponent* CEditorManager::CreateComponent(CGameObject* obj, size_t type)
{
	if(type == typeid(CSceneComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSceneComponent>();
		return component;
	}
	else if(type == typeid(CSpriteComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CSpriteComponent>();
		return component;
	}
	else if(type == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* component = obj->LoadComponent<CStaticMeshComponent>();
		return component;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

void CEditorManager::CreateAnimInstance(CSpriteComponent* comp, size_t type)
{
	if (type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		comp->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}
}

void CEditorManager::OnMouseLButtonDown(float deltaTime)
{
	if (mDragObj)
	{
		Vector2 mousePos = CInput::GetInst()->GetMousePos();
		mDragObj->SetStartPos(mousePos);

		mSpriteWindow->SetCropStartPos(mousePos);
	}
}

void CEditorManager::OnMouseLButtonPush(float deltaTime)
{
	if (mDragObj)
	{
		mDragObj->SetEndPos(CInput::GetInst()->GetMousePos());
	}
}

void CEditorManager::OnMouseLButtonUp(float deltaTime)
{
	if (mDragObj)
	{
		mSpriteWindow->SetCropEndPos(CInput::GetInst()->GetMousePos());
		mSpriteWindow->UpdateCropImage();
	}
}

void CEditorManager::OnDownArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		mDragObj->AddWorldPos(0.f,-1.f, 0.f);
		mSpriteWindow->MoveCropPos(0.f, -1.f);
	}
}

void CEditorManager::OnUpArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		mDragObj->AddWorldPos(0.f, 1.f, 0.f);
		mSpriteWindow->MoveCropPos(0.f, 1.f);
	}
}

void CEditorManager::OnLeftArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		mDragObj->AddWorldPos(-1.f, 0.f, 0.f);
		mSpriteWindow->MoveCropPos(-1.f, 0.f);
	}
}

void CEditorManager::OnRightArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		mDragObj->AddWorldPos(1.f, 0.f, 0.f);
		mSpriteWindow->MoveCropPos(1.f, 0.f);
	}
}

void CEditorManager::OnShiftRightArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		float width = abs(mDragObj->GetRelativeScale().x);
		mDragObj->AddWorldPos(width, 0.f, 0.f);

		mSpriteWindow->MoveCropPos(width, 0);
	}
}

void CEditorManager::OnShiftLeftArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		float width = abs(mDragObj->GetRelativeScale().x);
		mDragObj->AddWorldPos(-width, 0.f, 0.f);

		mSpriteWindow->MoveCropPos(-width, 0);
	}
}

void CEditorManager::OnShiftDownArrowKeyDown(float deltaTime)
{
	if (mDragObj)
	{
		float height = abs(mDragObj->GetRelativeScale().y);
		mDragObj->AddWorldPos(0.f, -height, 0.f);

		mSpriteWindow->MoveCropPos(0.f, -height);
	}
}

void CEditorManager::OnShiftUpArrowKeydown(float deltaTime)
{
	if (mDragObj)
	{
		float height = abs(mDragObj->GetRelativeScale().y);
		mDragObj->AddWorldPos(0.f, height, 0.f);

		mSpriteWindow->MoveCropPos(0.f, height);
	}
}

void CEditorManager::SetEditMode(eEditMode mode)
{
	meEditMode = mode;

	if (meEditMode == eEditMode::Sprite)
	{
		if (mDragObj)
		{
			mDragObj->Destroy();
		}

		mDragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		mDragObj->SetWorldScale(0.f, 0.f, 1.f);
	}
}
