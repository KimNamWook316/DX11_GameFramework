#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/StructuredBuffer.h"

CParticleComponent::CParticleComponent()	:
	mSpawnTime(0.f),
	mSpawnTimeMax(0.01f),
	mInfo{},
	mInfoShared{},
	mCBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	mbIsRender = true;
}

CParticleComponent::CParticleComponent(const CParticleComponent& com)	:
	CSceneComponent(com)
{
	mSpawnTime = 0.f;
	mSpawnTimeMax = com.mSpawnTimeMax;

	mParticle = com.mParticle;

	// 현재 있는 구조화 버퍼들 삭제하고
	size_t bufferCount = mVecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; ++i)
	{
		SAFE_DELETE(mVecStructuredBuffer[i]);
	}

	mVecStructuredBuffer.clear();

	// 파티클에서 복제해온다.
	if (mParticle)
	{
		mParticle->CloneStructuredBuffer(mVecStructuredBuffer);
		mUpdateShader = mParticle->GetUpdateShader();
		SAFE_DELETE(mCBuffer);
		mCBuffer = mParticle->CloneConstantBuffer();
	}
}

CParticleComponent::~CParticleComponent()
{
	size_t size = mVecStructuredBuffer.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecStructuredBuffer[i]);
	}
	SAFE_DELETE(mCBuffer);
}

bool CParticleComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		assert(false);
		return false;
	}
	
	// 점 메쉬
	mMesh = mScene->GetResource()->FindMesh("ParticlePointMesh");
	return true;
}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

void CParticleComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	mSpawnTime += deltaTime;

	// 생성 쿨타임이 되면 UpdateShder에서 죽어있는 Particle중 하나를 살리게
	if (mSpawnTime >= mSpawnTimeMax)
	{
		mSpawnTime -= mSpawnTimeMax;
		mCBuffer->SetSpawnEnable(1);
	}
	else
	{
		mCBuffer->SetSpawnEnable(0);
	}
}

void CParticleComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);

	CParticleConstantBuffer* CBuf = mParticle->GetCBuffer();

	// Update Shader를 동작한다.
	Vector3 startMin, startMax;

	startMin = GetWorldPos() + CBuf->GetStartMin();
	startMax = GetWorldPos() + CBuf->GetStartMax();

	mCBuffer->SetStartMin(startMin);
	mCBuffer->SetStartMax(startMax);

	mCBuffer->UpdateCBuffer();

	size_t bufferCount = mVecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; ++i)
	{
		// RW 구조화 버퍼를 파이프라인에 연결
		mVecStructuredBuffer[i]->SetShader();
	}

	// Update Shader에서는 64, 1, 1 을 한 그룹으로 사용하는 중
	// 생성되어야 할 파티클의 전체 수에서 64를 나눈다. 만약 64개가 최대 파티클 수라면
	// 필요한 그룹의 수는 1개이다. 64개 미만에서는 0이 나오므로 1을 더해줘야 함
	// 100개일 경우 그룹은 2개, 이 때 스레드는 128개가 생성되지만, 쉐이더 코드에서 나머지 28개가 생성되지 않게 막아주고 있음
	int groupCount = mParticle->GetSpawnCountMax() / 64 == 0 ? 1 : mParticle->GetSpawnCountMax() / 64;

	// RW 구조화 버퍼를 Compute Shader를 통해 Update
	mUpdateShader->Excute(groupCount, 1, 1);
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::Render()
{
	CSceneComponent::Render();

	size_t bufferCount = mVecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; ++i)
	{
		// gParticleArraySRV, gParticleSharedSRV에 구조화버퍼 보내고, Geometry Shader에 바인드 
		mVecStructuredBuffer[i]->SetShader(30 + (int)i, (int)eBufferShaderTypeFlags::Geometry);
	}
	
	if (mMaterial)
	{
		mMaterial->Render();
	}

	// 인스턴싱을 이용해서 그린다.


	for (size_t i = 0; i < bufferCount; ++i)
	{
		mVecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)eBufferShaderTypeFlags::Geometry);
	}

	if (mMaterial)
	{
		mMaterial->Reset();
	}
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

void CParticleComponent::SetParticle(const std::string& name)
{
	SetParticle(mScene->GetResource()->FindParticle(name));
}

void CParticleComponent::SetParticle(CParticle* particle)
{
	// 새로 받은 파티클에 맞춰 멤버 초기화
	mParticle = particle;

	mMaterial = mParticle->CloneMaterial();

	size_t bufferCount = mVecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; ++i)
	{
		SAFE_DELETE(mVecStructuredBuffer[i]);
	}

	mVecStructuredBuffer.clear();

	mParticle->CloneStructuredBuffer(mVecStructuredBuffer);
	mUpdateShader = mParticle->GetUpdateShader();
	SAFE_DELETE(mCBuffer);
	mCBuffer = mParticle->CloneConstantBuffer();
	mSpawnTimeMax = mParticle->GetSpawnTime();
}

void CParticleComponent::SetSpawnTime(const float time)
{
	mSpawnTimeMax = time;
	mParticle->SetSpawnTime(time);
}
