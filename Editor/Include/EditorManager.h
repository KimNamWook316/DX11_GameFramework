#pragma once

#include "GameInfo.h"

enum class eEditMode
{
	Scene,
	Sprite,
};

class CEditorManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

public:
	void CreateSceneMode(class CScene* scene, size_t type);
	class CGameObject* CreateObject(class CScene* scene, size_t type);
	class CComponent* CreateComponent(class CGameObject* obj, size_t type);
	void CreateAnimInstance(class CSpriteComponent* comp, size_t type);

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

public:
	eEditMode GetEditMode()
	{
		return meEditMode;
	}

	class CDragObject* GetDragObject()
	{
		return mDragObj;
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
};

