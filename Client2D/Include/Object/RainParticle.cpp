#include "RainParticle.h"
#include "Component/ParticleComponent.h"

CRainParticle::CRainParticle()
{
	SetTypeID<CRainParticle>();
}

CRainParticle::CRainParticle(const CRainParticle& obj)	:
	CGameObject(obj)
{
	mParticle = (CParticleComponent*)FindComponent("Particle");
}

CRainParticle::~CRainParticle()
{
}

bool CRainParticle::Init()
{
	mParticle = CreateComponent<CParticleComponent>("Particle");

	SetRootSceneComponent(mParticle);

	mParticle->SetRelativePos(0.f, 0.f, 0.f);

	mParticle->SetParticle("Rain");

	return true;
}

CRainParticle* CRainParticle::Clone()
{
	return new CRainParticle(*this);
}
