#include "Particle.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"
#include "../Shader/StructuredBuffer.h"

CParticle::CParticle()	:
	mScene(nullptr),
	mCBuffer(nullptr),
	mSpawnTime(0.001f),
	mb2D(true),
	mSpawnCountMax(100)
{
}

CParticle::CParticle(const CParticle& particle)
{
	mCBuffer = particle.mCBuffer->Clone();
	mUpdateShader = particle.mUpdateShader;
}

CParticle::~CParticle()
{
	size_t size = mVecStructuredBuffer.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecStructuredBuffer[i]);
	}
	
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

	// RWStructuredBuffer ( register 주목 )
	AddStructuredBuffer("ParticleInfo", sizeof(ParticleInfo), mSpawnCountMax, 0);
	AddStructuredBuffer("ParticleInfoShared", sizeof(ParticleInfoShared), 1, 1);

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

bool CParticle::ResizeBuffer(const std::string& name, unsigned int size, unsigned int count, int reg, bool bDynamic, int structuredBufferShaderType)
{
	size_t vecSize = mVecStructuredBuffer.size();

	for (size_t i = 0; i < vecSize; ++i)
	{
		// 해당 이름과 같은 버퍼를 다시 만든다.
		if (name == mVecStructuredBuffer[i]->GetName())
		{
			mVecStructuredBuffer[i]->Init(name, size, count, reg, bDynamic, structuredBufferShaderType);
			return true;
		}
	}
	return false;
}

// 구조화 버퍼 복사 함수
void CParticle::CloneStructuredBuffer(std::vector<CStructuredBuffer*>& outVecBuffer)
{
	size_t size = mVecStructuredBuffer.size();

	for (size_t i = 0; i < size; ++i)
	{
		CStructuredBuffer* buffer = mVecStructuredBuffer[i]->Clone();

		outVecBuffer.push_back(buffer);
	}
}

void CParticle::SetSpawnCountMax(const unsigned int count)
{
	mCBuffer->SetSpawnCountMax(count);

	mSpawnCountMax = count;

	// 총 Spawn 개수가 늘었으므로, 구조화 버퍼 개수또한 갱신되어야 함
	ResizeBuffer("ParticleInfo", sizeof(ParticleInfo), mSpawnCountMax, 0);
}
