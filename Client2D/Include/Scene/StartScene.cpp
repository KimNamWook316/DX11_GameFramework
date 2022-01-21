#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	mStartWidget = mScene->GetViewport()->CreateWidgetWindow<CStartWidget>("StartWidget");
	return true;
}
