#pragma once

#include "GameInfo.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

public:
	void CreateSceneMode(class CScene* scene, size_t type);
	void CreateObject(class CScene* scene, size_t type);

	DECLARE_SINGLE(CClientManager)
};

