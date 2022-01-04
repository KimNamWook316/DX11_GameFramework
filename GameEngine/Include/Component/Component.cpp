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
	mScene = nullptr;
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

void CComponent::Destroy()
{
	CRef::Destroy();
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Start()
{
}

void CComponent::Save(FILE* fp)
{
	CRef::Save(fp);
	fwrite(&meComponentType, sizeof(eComponentType), 1, fp);
}

void CComponent::Load(FILE* fp)
{
	CRef::Load(fp);
	fread(&meComponentType, sizeof(eComponentType), 1, fp);
}
