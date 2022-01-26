#include "Particle.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"
#include "../Shader/StructuredBuffer.h"

CParticle::CParticle()	:
	mScene(nullptr),
	mCBuffer(nullptr),
	mSpawnTime(0.001f),
	mb2D(true)
{
}

CParticle::CParticle(const CParticle& particle)
{
	mCBuffer = particle.mCBuffer->Clone();
	mUpdateShader = particle.mUpdateShader;
}

CParticle::~CParticle()
{
	SAFE_DELETE(mCBuffer);
}

bool CParticle::Init()
{
	mCBuffer = new CParticleConstantBuffer;

	if (!mCBuffer->Init())
	{
		assert(false);
		return false;
	}

	if (mScene)
	{
		mUpdateShader = (CParticleUpdateShader*)mScene->GetResource()->FindShader("ParticleUpdateShader");
	}
	else
	{
		mUpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader("ParticleUpdateShader");
	}

	return true;
}

void CParticle::AddStructuredBuffer(const std::string& name, unsigned int size, unsigned int count, int reg, bool bDynamic, int structuredBufferShaderType)
{
	CStructuredBuffer* buffer = new CStructuredBuffer;

	if (!buffer->Init(name, size, count, reg, bDynamic, structuredBufferShaderType))
	{
		assert(false);
		return;
	}
	mVecStructuredBuffer.push_back(buffer);
}
