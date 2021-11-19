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
	// 복사되면서 복사 생성자 호출, CRef의 자식이기 때문에 SharedPtr로 관리되며, 레퍼런스 카운트 체크함
	return new CSceneComponent(*this);
}
