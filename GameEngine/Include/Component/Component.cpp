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
	mObject = nullptr; // ������Ʈ�� ���� �����ϴ� ���� ������Ʈ���� ����
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
