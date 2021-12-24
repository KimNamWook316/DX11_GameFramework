#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CDefaultScene::CDefaultScene()
{
    SetTypeID<CDefaultScene>();
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
    if (!loadAnimationSequence2D())
    {
        return false;
    }
    return true;
}

bool CDefaultScene::loadAnimationSequence2D()
{
    mScene->GetResource()->LoadSequence2D("one.sqc");
    mScene->GetResource()->LoadSequence2D("two.sqc");
    return true;
}
