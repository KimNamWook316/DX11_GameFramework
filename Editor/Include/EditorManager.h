#pragma once

#include "GameInfo.h"

enum class eEditMode
{
	Scene,
	Sprite,
	TileMap,
};

class CEditorManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();
	void RecreateEditorCameraDragObj();

public:
	void OnCreateSceneMode(class CScene* scene, size_t type);
	class CGameObject* OnCreateObject(class CScene* scene, size_t type);
	class CComponent* OnCreateComponent(class CGameObject* obj, size_t type);
	void OnCreateAnimInstance(class CSpriteComponent* comp, size_t type);
	void OnCreateState(class CStateComponent* comp, size_t type);

public:
	void OnMouseLButtonDown(float deltaTime);
	void OnMouseLButtonPush(float deltaTime);
	void OnMouseLButtonUp(float deltaTime);
	void OnDownArrowKeyDown(float deltaTime);
	void OnUpArrowKeyDown(float deltaTime);
	void OnLeftArrowKeyDown(float deltaTime);
	void OnRightArrowKeyDown(float deltaTime);
	void OnShiftRightArrowKeyDown(float deltaTime);
	void OnShiftLeftArrowKeyDown(float deltaTime);
	void OnShiftDownArrowKeyDown(float deltaTime);
	void OnShiftUpArrowKeydown(float deltaTime);
	void OnScrollDown(float deltaTime);
	void OnScrollUp(float deltaTime);
	void OnScrollLeft(float deltaTime);
	void OnScrollRight(float deltaTime);
	void OnF2Down(float deltaTime);

public:
	eEditMode GetEditMode()
	{
		return meEditMode;
	}

	class CDragObject* GetDragObject()
	{
		return mDragObj;
	}

	bool GetLButtonPush() const
	{
		return mbMousePush;
	}

	void SetCamera(class CCameraObject* camera)
	{
		mCameraObj = camera;
	}

	void SetEditMode(eEditMode mode);

	DECLARE_SINGLE(CEditorManager)

private:
	eEditMode meEditMode;

	class CSpriteWindow* mSpriteWindow;
	class CObjectHierachyWindow* mObjectHierachyWindow;
	class CDetailWindow* mDetailWindow;
	class CEditorMenuWindow* mEditorMenuWindow;

	class CDragObject* mDragObj;
	class CCameraObject* mCameraObj;
	
	bool mbMousePush;
};

