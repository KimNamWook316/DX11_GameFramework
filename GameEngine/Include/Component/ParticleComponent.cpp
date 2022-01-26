#include "ParticleComponent.h"

CParticleComponent::CParticleComponent()
{
	SetTypeID<CParticleComponent>();
	mbIsRender = true;
}

CParticleComponent::CParticleComponent(const CParticleComponent& com)	:
	CSceneComponent(com)
{
}

CParticleComponent::~CParticleComponent()
{
}

bool CParticleComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		assert(false);
		return false;
	}
}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

void CParticleComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CParticleComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::Render()
{
	CSceneComponent::Render();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}

void CParticleComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);
}

void CParticleComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
}
