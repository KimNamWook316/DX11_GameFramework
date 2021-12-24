#include "ColliderBox2D.h"

CColliderBox2D::CColliderBox2D()
{
	SetTypeID<CColliderBox2D>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;
	meColliderType = eColliderType::Box2D;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& com)	:
	CColliderComponent(com)
{
}

CColliderBox2D::~CColliderBox2D()
{
}

bool CColliderBox2D::Init()
{
	if (!CColliderComponent::Init())
	{
		assert(false);
		return false;
	}
	return true;
}

void CColliderBox2D::Start()
{
	CColliderComponent::Start();
}

void CColliderBox2D::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderBox2D::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);
}

void CColliderBox2D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox2D::Render()
{
	CColliderComponent::Render();
}

void CColliderBox2D::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderBox2D* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

void CColliderBox2D::Save(FILE* fp)
{
	CColliderComponent::Save(fp);
}

void CColliderBox2D::Load(FILE* fp)
{
	CColliderComponent::Load(fp);
}
