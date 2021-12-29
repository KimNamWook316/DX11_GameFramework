#pragma once

#include "ConstantBufferBase.h"

class CColliderConstantBuffer :
    public CConstantBufferBase
{
public:
    CColliderConstantBuffer();
    CColliderConstantBuffer(const CColliderConstantBuffer& buffer);
    virtual ~CColliderConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer();
    virtual CColliderConstantBuffer* Clone();

public:
    void SetColliderColor(const Vector4& color)
    {
        mBufferData.Color = color;
    }

    void SetColliderWVP(const Matrix& matWVP)
    {
        mBufferData.MatWVP = matWVP;
    }

protected:
    ColliderCBuffer mBufferData;
};

