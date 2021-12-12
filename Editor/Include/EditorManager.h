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
	eEditMode GetEditMode()
	{
		return meEditMode;
	}

	void SetEditMode(eEditMode mode)
	{
		meEditMode = mode;
	}

	DECLARE_SINGLE(CEditorManager)

private:
	eEditMode meEditMode;
};

