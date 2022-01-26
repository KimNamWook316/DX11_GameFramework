#include "ParticleManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
}

bool CParticleManager::Init()
{
	return true;
}

bool CParticleManager::CreateParticle(const std::string& name)
{
	CParticle* particle = FindParticle(name);

	if (particle)
	{
		return true;
	}
	
	particle = new CParticle;
	particle->SetName(name);

	if (!particle->Init())
	{
		SAFE_RELEASE(particle);
		assert(false);
		return false;
	}
	mMapParticle.insert(std::make_pair(name, particle));
	return true;
}

CParticle* CParticleManager::FindParticle(const std::string& name)
{
	auto iter = mMapParticle.find(name);

	if (iter == mMapParticle.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CParticleManager::ReleaseParticle(const std::string& name)
{
	auto iter = mMapParticle.find(name);

	if (iter != mMapParticle.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMapParticle.erase(iter);
		}
	}
}
