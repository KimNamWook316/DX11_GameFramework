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
	class CGameObject* OnCreateObject(class CScene* scene, size_t type);
	void OnCreateAnimInstance(class CSpriteComponent* comp, size_t type);
	class CComponent* OnCreateComponent(class CGameObject* obj, size_t type);
	void OnCreateState(class CStateComponent* comp, size_t type);

	DECLARE_SINGLE(CClientManager)
};

