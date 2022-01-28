#pragma once

#include "ConstantBufferBase.h"

class CParticleConstantBuffer :
    public CConstantBufferBase
{
public:
    CParticleConstantBuffer();
    CParticleConstantBuffer(const CParticleConstantBuffer& buffer);
    virtual ~CParticleConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CParticleConstantBuffer* Clone() override;

public:
	const Vector3& GetStartMin() const
	{
		return mBufferData.StartMin;
	}

	const Vector3& GetStartMax() const
	{
		return mBufferData.StartMax;
	}
	
	int GetSpawnCount() const
	{
		return mBufferData.SpawnCountMax;
	}

public:
	void SetSpawnEnable(const unsigned int count)
	{
		mBufferData.SpawnEnable = count;
	}

	void SetSpawnCountMax(const unsigned int count)
	{
		mBufferData.SpawnCountMax = count;
	}
	
	void SetStartMin(const Vector3& val)
	{
		mBufferData.StartMin = val;
	}

	void SetStartMax(const Vector3& val)
	{
		mBufferData.StartMax = val;
	}

	void SetScaleMin(const Vector3& val)
	{
		mBufferData.ScaleMin = val;
	}

	void SetScaleMax(const Vector3& val)
	{
		mBufferData.ScaleMax = val;
	}

	void SetMoveDir(const Vector3& val)
	{
		mBufferData.MoveDir = val;
	}

	void SetMoveAngle(const Vector3& val)
	{
		mBufferData.MoveAngle = val;
	}

	void SetColorMin(const Vector4& val)
	{
		mBufferData.ColorMin = val;
	}

	void SetColorMax(const Vector4& val)
	{
		mBufferData.ColorMax = val;
	}

	void SetLifeTimeMin(const float val)
	{
		mBufferData.LifeTimeMin = val;
	}

	void SetLifeTimeMax(const float val)
	{
		mBufferData.LifeTimeMax = val;
	}

	void SetSpeedMin(const float val)
	{
		mBufferData.SpeedMin = val;
	}

	void SetSpeedMax(const float val)
	{
		mBufferData.SpeedMax = val;
	}

	void SetIsMove(bool bMove)
	{
		mBufferData.IsMove = bMove ? 1 : 0;
	}
	
	void SetIsGravity(bool bGravity)
	{
		mBufferData.IsGravity = bGravity ? 1 : 0;
	}
	
	void SetIs2D(bool b2D)
	{
		mBufferData.Is2D = b2D ? 1 : 0;
	}

private:
    ParticleCBuffer mBufferData;
};

