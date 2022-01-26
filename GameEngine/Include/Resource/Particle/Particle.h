#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"
#include "../Shader/ParticleUpdateShader.h"
#include "../Shader/ParticleConstantBuffer.h"

class CParticle :
    public CRef
{
    friend class CParticleManager;

private:
    CParticle();
    CParticle(const CParticle& particle);
    ~CParticle();

public:
    bool Init();
    void AddStructuredBuffer(const std::string& name, unsigned int size, unsigned int count,
        int reg, bool bDynamic = false,
        int structuredBufferShaderType = (int)eBufferShaderTypeFlags::Compute);

public:
	void SetSpawnEnable(const unsigned int count)
	{
		mCBuffer->SetSpawnEnable(count);
	}

	void SetSpawnCountMax(const unsigned int count)
	{
		mCBuffer->SetSpawnCountMax(count);
	}
	
	void SetStartMin(const Vector3& val)
	{
		mCBuffer->SetStartMin(val);
	}

	void SetStartMax(const Vector3& val)
	{
		mCBuffer->SetStartMax(val);
	}

	void SetScaleMin(const Vector3& val)
	{
		mCBuffer->SetScaleMin(val);
	}

	void SetScaleMax(const Vector3& val)
	{
		mCBuffer->SetScaleMax(val);
	}

	void SetMoveDir(const Vector3& val)
	{
		mCBuffer->SetMoveDir(val);
	}

	void SetMoveAngle(const Vector3& val)
	{
		mCBuffer->SetMoveAngle(val);
	}

	void SetColorMin(const Vector4& val)
	{
		mCBuffer->SetColorMin(val);
	}

	void SetColorMax(const Vector4& val)
	{
		mCBuffer->SetColorMax(val);
	}

	void SetLifeTimeMin(const float val)
	{
		mCBuffer->SetLifeTimeMin(val);
	}

	void SetLifeTimeMax(const float val)
	{
		mCBuffer->SetLifeTimeMax(val);
	}

	void SetSpeedMin(const float val)
	{
		mCBuffer->SetSpeedMin(val);
	}

	void SetSpeedMax(const float val)
	{
		mCBuffer->SetSpeedMax(val);
	}

	void SetIsMove(bool bMove)
	{
		mCBuffer->SetIsMove(bMove);
	}
	
	void SetIsGravity(bool bGravity)
	{
		mCBuffer->SetIsGravity(bGravity);
	}
	
	void SetIs2D(bool b2D)
	{
		mCBuffer->SetIs2D(b2D);
	}

private:
    class CScene* mScene;
    std::vector<class CStructuredBuffer*> mVecStructuredBuffer;
    CSharedPtr<CMaterial> mMaterial;
    CSharedPtr<CParticleUpdateShader> mUpdateShader;
    ParticleInfo mInfo;
    ParticleInfoShared mInfoShared;
    CParticleConstantBuffer* mCBuffer;
    float mSpawnTime;
    bool mb2D;
};

