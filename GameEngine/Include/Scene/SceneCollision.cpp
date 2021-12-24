#include "SceneCollision.h"
#include "../Engine.h"

CSceneCollision::CSceneCollision()
{
}

CSceneCollision::~CSceneCollision()
{
}

bool CSceneCollision::Init()
{
	// 2D인 경우 Z축 공간분할 X
	if (CEngine::GetInst()->GetEngineSpace() == eEngineSpace::Space2D)
	{

	}

	else
	{

	}
	return true;
}

void CSceneCollision::Start()
{
}
