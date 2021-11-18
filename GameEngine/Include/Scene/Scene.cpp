#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Update(float deltaTime)
{
	mMode->Update(deltaTime);
}

void CScene::PostUpdate(float deltaTime)
{
	mMode->PostUpdate(deltaTime);
}
