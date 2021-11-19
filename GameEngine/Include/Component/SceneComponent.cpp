#include "SceneComponent.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	meComponentType = Component_Type::SCENE_COMP;
	mbIsRender = false;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com),
	mbIsRender(false)
{
}

CSceneComponent::~CSceneComponent()
{
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(float DeltaTime)
{
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
}

void CSceneComponent::PrevRender()
{
}

void CSceneComponent::Render()
{
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	// ����Ǹ鼭 ���� ������ ȣ��, CRef�� �ڽ��̱� ������ SharedPtr�� �����Ǹ�, ���۷��� ī��Ʈ üũ��
	return new CSceneComponent(*this);
}
