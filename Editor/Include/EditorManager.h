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
	void OnMouseLButtonDown(float deltaTime);
	void OnMouseLButtonPush(float deltaTime);
	void OnMouseLButtonUp(float deltaTime);
	void OnDownArrowKeyDown(float deltaTime);
	void OnUpArrowKeyDown(float deltaTime);
	void OnLeftArrowKeyDown(float deltaTime);
	void OnRightArrowKeyDown(float deltaTime);

public:
	eEditMode GetEditMode()
	{
		return meEditMode;
	}

	void SetEditMode(eEditMode mode);

	DECLARE_SINGLE(CEditorManager)

private:
	eEditMode meEditMode;
	class CSpriteWindow* mIMGUISpriteWindow;
	class CDragObject* mDragObj;
};

