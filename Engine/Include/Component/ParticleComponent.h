#pragma once

#include "SceneComponent.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/ParticleRenderShader.h"
#include "../Resource/Shader/ParticleUpdateShader.h"
#include "../Resource/Shader/ParticleConstantBuffer.h"
#include "../Resource/Material/Material.h"

class CParticleComponent :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CParticleComponent();
	CParticleComponent(const CParticleComponent& com);
	virtual ~CParticleComponent();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CParticleComponent* Clone() override;

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
	void SetParticle(const std::string& name);
	void SetParticle(CParticle* particle);
	void SetSpawnTime(const float time);

protected:
	CSharedPtr<CParticle> mParticle;
	std::vector<class CStructuredBuffer*> mVecStructuredBuffer;
	CSharedPtr<CMesh> mMesh;
	CSharedPtr<CMaterial> mMaterial;
	CSharedPtr<CParticleUpdateShader> mUpdateShader;
	CParticleConstantBuffer* mCBuffer;
	ParticleInfo mInfo;
	ParticleInfoShared mInfoShared;
	float mSpawnTime;
	float mSpawnTimeMax;
};

