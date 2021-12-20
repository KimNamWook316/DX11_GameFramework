#include "EditorManager.h"
#include "Engine.h"
#include "Resource.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Scene/DefaultScene.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Object/SpriteEditObject.h"
#include "Window/SpriteWindow.h"
#include "Window/ObjectHierachyWindow.h"
#include "Window/DetailWindow.h"
#include "Window/EditorMenuWindow.h"
#include "Object/DragObject.h"

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
	
	// SceneManager CallBack
	CSceneManager::GetInst()->SetCreateSceneModeCallBack<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectCallBack<CEditorManager>(this, &CEditorManager::CreateObject);

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

void CEditorManager::CreateSceneMode(CScene*, size_t type)
{
}

void CEditorManager::CreateObject(CScene*, size_t type)
{
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
