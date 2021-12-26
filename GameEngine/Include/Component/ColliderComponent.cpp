#include "ColliderComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
}

CColliderComponent::~CColliderComponent()
{
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		assert(false);
		return false;
	}
	return true;
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

void CColliderComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CColliderComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CColliderComponent::CheckCollision()
{
	// 자신을 씬 충돌 매니저에 등록
	mScene->GetCollision()->AddCollider(this);
	
	CSceneComponent::CheckCollision();
}

void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColliderComponent::Render()
{
	CSceneComponent::Render();
}

void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderComponent* CColliderComponent::Clone()
{
	return new CColliderComponent(*this);
}

void CColliderComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);
	fwrite(&meColliderType, sizeof(eColliderType), 1, fp);
	fwrite(&mOffset, sizeof(Vector3), 1, fp);
	fwrite(&mMinPos, sizeof(Vector3), 1, fp);
	fwrite(&mMaxPos, sizeof(Vector3), 1, fp);
}

void CColliderComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
	fread(&meColliderType, sizeof(eColliderType), 1, fp);
	fread(&mOffset, sizeof(Vector3), 1, fp);
	fread(&mMinPos, sizeof(Vector3), 1, fp);
	fread(&mMaxPos, sizeof(Vector3), 1, fp);
}
