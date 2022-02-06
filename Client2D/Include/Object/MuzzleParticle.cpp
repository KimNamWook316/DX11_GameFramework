#include "MuzzleParticle.h"
#include "Component/ParticleComponent.h"
#include "Input.h"
#include "Scene/Scene.h"

CMuzzleParticle::CMuzzleParticle()
{
	SetTypeID<CMuzzleParticle>();
}

CMuzzleParticle::CMuzzleParticle(const CMuzzleParticle& obj)	:
	CGameObject(obj)
{
	mFlashParticle = (CParticleComponent*)FindComponent("Flash");
	mFragmentParticle = (CParticleComponent*)FindComponent("Fragment");
}

CMuzzleParticle::~CMuzzleParticle()
{
}

bool CMuzzleParticle::Init()
{
	mLifeSpan = 0.2f;

	mFlashParticle = CreateComponent<CParticleComponent>("Flash");
	mFragmentParticle = CreateComponent<CParticleComponent>("Fragment");
	SetRootSceneComponent(mFlashParticle);
	mFlashParticle->AddChild(mFragmentParticle);
	mFlashParticle->SetRelativePos(0.f, 0.f, 0.f);
	mFragmentParticle->SetRelativePos(-30.f, 0.f, 0.f);
	mFlashParticle->SetParticle("Flash");
	mFragmentParticle->SetParticle("Fragment");

	return true;
}

CMuzzleParticle* CMuzzleParticle::Clone()
{
	return new CMuzzleParticle(*this);
}
