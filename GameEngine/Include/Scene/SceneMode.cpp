#include "SceneMode.h"

CSceneMode::CSceneMode()	:
	mScene(nullptr)
{
	SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

bool CSceneMode::Init()
{
	return true;
}

void CSceneMode::Update(float deltaTime)
{
}

void CSceneMode::PostUpdate(float deltaTime)
{
}
