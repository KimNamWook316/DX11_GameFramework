#include "BubbleParticle.h"
#include "Component/ParticleComponent.h"

CBubbleParticle::CBubbleParticle()
{
	SetTypeID<CBubbleParticle>();
}

CBubbleParticle::CBubbleParticle(const CBubbleParticle& obj)	:
	CGameObject(obj)
{
	mParticle = (CParticleComponent*)FindComponent("Particle");
}

CBubbleParticle::~CBubbleParticle()
{
}

bool CBubbleParticle::Init()
{
	mParticle = CreateComponent<CParticleComponent>("Particle");

	SetRootSceneComponent(mParticle);

	mParticle->SetRelativePos(200.f, 50.f, 0.f);

	mParticle->SetParticle("Bubble");

	return true;
}

CBubbleParticle* CBubbleParticle::Clone()
{
	return new CBubbleParticle(*this);
}
