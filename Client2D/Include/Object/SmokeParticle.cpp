#include "SmokeParticle.h"
#include "Component/ParticleComponent.h"

CSmokeParticle::CSmokeParticle()
{
	SetTypeID<CSmokeParticle>();
}

CSmokeParticle::CSmokeParticle(const CSmokeParticle& obj)	:
	CGameObject(obj)
{
	mParticle = (CParticleComponent*)FindComponent("SmokeParticle");
}

CSmokeParticle::~CSmokeParticle()
{
}

bool CSmokeParticle::Init()
{
	mParticle = CreateComponent<CParticleComponent>("SmokeParticle");

	SetRootSceneComponent(mParticle);

	mParticle->SetWorldPos(500.f, 50.f, 0.f);

	mParticle->SetParticle("Smoke");
	return true;
}

CSmokeParticle* CSmokeParticle::Clone()
{
	return new CSmokeParticle(*this);
}
