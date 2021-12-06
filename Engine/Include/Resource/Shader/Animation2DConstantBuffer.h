#pragma once

#include "ConstantBufferBase.h"

class CAnimation2DConstantBuffer :
    public CConstantBufferBase
{
public:
    CAnimation2DConstantBuffer();
    CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& buffer);
    virtual ~CAnimation2DConstantBuffer();

public:
    virtual bool Init();
    virtual void UpdateCBuffer();
    virtual CAnimation2DConstantBuffer* Clone();

public:
    void SetAnimation2DType(eImageType type)
    {
        mBufferData.Animation2DType = (int)type;
    }

    void SetStartUV(const Vector2& startUV)
    {
        mBufferData.Animation2DStartUV = startUV;
    }

    void SetEndUV(const Vector2& endUV)
    {
        mBufferData.Animation2DEndUV = endUV;
    }
protected:
    Animation2DCBuffer mBufferData;
};

