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
	// 2D�� ��� Z�� �������� X
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
