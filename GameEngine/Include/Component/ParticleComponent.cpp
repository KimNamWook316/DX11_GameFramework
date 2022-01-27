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

	// ���� �ִ� ����ȭ ���۵� �����ϰ�
	size_t bufferCount = mVecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; ++i)
	{
		SAFE_DELETE(mVecStructuredBuffer[i]);
	}

	mVecStructuredBuffer.clear();

	// ��ƼŬ���� �����ؿ´�.
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
	
	// �� �޽�
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

	// ���� ��Ÿ���� �Ǹ� UpdateShder���� �׾��ִ� Particle�� �ϳ��� �츮��
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

	// Update Shader�� �����Ѵ�.
	Vector3 startMin, startMax;

	startMin = GetWorldPos() + CBuf->GetStartMin();
	startMax = GetWorldPos() + CBuf->GetStartMax();

	mCBuffer->SetStartMin(startMin);
	mCBuffer->SetStartMax(startMax);

	mCBuffer->UpdateCBuffer();

	size_t bufferCount = mVecStructuredBuffer.size();

	for (size_t i = 0; i < bufferCount; ++i)
	{
		// RW ����ȭ ���۸� ���������ο� ����
		mVecStructuredBuffer[i]->SetShader();
	}

	// Update Shader������ 64, 1, 1 �� �� �׷����� ����ϴ� ��
	// �����Ǿ�� �� ��ƼŬ�� ��ü ������ 64�� ������. ���� 64���� �ִ� ��ƼŬ �����
	// �ʿ��� �׷��� ���� 1���̴�. 64�� �̸������� 0�� �����Ƿ� 1�� ������� ��
	// 100���� ��� �׷��� 2��, �� �� ������� 128���� ����������, ���̴� �ڵ忡�� ������ 28���� �������� �ʰ� �����ְ� ����
	int groupCount = mParticle->GetSpawnCountMax() / 64 == 0 ? 1 : mParticle->GetSpawnCountMax() / 64;

	// RW ����ȭ ���۸� Compute Shader�� ���� Update
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
		// gParticleArraySRV, gParticleSharedSRV�� ����ȭ���� ������, Geometry Shader�� ���ε� 
		mVecStructuredBuffer[i]->SetShader(30 + (int)i, (int)eBufferShaderTypeFlags::Geometry);
	}
	
	if (mMaterial)
	{
		mMaterial->Render();
	}

	// �ν��Ͻ��� �̿��ؼ� �׸���.


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
	// ���� ���� ��ƼŬ�� ���� ��� �ʱ�ȭ
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
