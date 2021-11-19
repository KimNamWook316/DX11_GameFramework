#include "Component.h"

CComponent::CComponent() :
	mScene(nullptr),
	mObject(nullptr)
{
}

CComponent::CComponent(const CComponent& com)
{
	*this = com; // shallow copy
	mRefCount = 0;
	mObject = nullptr; // 오브젝트와 씬은 생성하는 게임 오브젝트에서 지정
}

CComponent::~CComponent()
{
}

void CComponent::SetScene(CScene* scene)
{
	mScene = scene;
}

void CComponent::SetGameObject(CGameObject* object)
{
	mObject = object;
}

bool CComponent::Init()
{
	return true;
}
