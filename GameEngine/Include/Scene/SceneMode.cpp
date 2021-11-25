#include "SceneMode.h"
#include "../GameObject/GameObject.h"

CSceneMode::CSceneMode()	:
	mScene(nullptr)
{
	SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::SetPlayerObj(CGameObject* obj)
{
	mPlayerObj = obj;
}

bool CSceneMode::Init()
{
	return true;
}

void CSceneMode::Start()
{
}

void CSceneMode::Update(float deltaTime)
{
}

void CSceneMode::PostUpdate(float deltaTime)
{
}
