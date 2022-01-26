#pragma once

#include "ConstantBufferBase.h"

class CGlobalConstantBuffer :
    public CConstantBufferBase
{
public:
    CGlobalConstantBuffer();
    CGlobalConstantBuffer(const CGlobalConstantBuffer& buffer);
    virtual ~CGlobalConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CGlobalConstantBuffer* Clone() override;

public:
    void SetDeltaTime(const float deltaTime)
    {
        mBufferData.DeltaTime = deltaTime;
    }

    void SetAccTime(const float acc)
    {
        mBufferData.AccTime = acc;
    }

    void SetResolution(const Resolution& res)
    {
        mBufferData.Resolution.x = (float)res.Width;
        mBufferData.Resolution.y = (float)res.Height;
    }

    void SetNoiseResolution(const Vector2& res)
    {
        mBufferData.NoiseResolution = res;
    }

private:
    GlobalCBuffer mBufferData;
};

