#pragma once

#include "ConstantBufferBase.h"

class CSpaceConstantBuffer :
    public CConstantBufferBase
{
public:
    CSpaceConstantBuffer();
    CSpaceConstantBuffer(const CSpaceConstantBuffer& buffer);
    virtual ~CSpaceConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CSpaceConstantBuffer* Clone() override;

public:
    void SetRhombusLB(const Vector3& pos)
    {
        mBufferData.RhombusLBPos = pos;
    }

    void SetRhombusLT(const Vector3& pos)
    {
        mBufferData.RhombusLTPos = pos;
    }

    void SetRhombusRT(const Vector3& pos)
    {
        mBufferData.RhombusRTPos = pos;
    }

    void SetRhombusRB(const Vector3& pos)
    {
        mBufferData.RhombusRBPos = pos;
    }

    void SetColor(const Vector4& color)
    {
        mBufferData.Color = color;
    }

    void SetColor(const float r, const float g, const float b, const float a)
    {
        mBufferData.Color = Vector4(r, g, b, a);
    }

    void SetMatWVP(const Matrix& mat)
    {
        mBufferData.MatWVP = mat;
    }

    void SetMatVP(const Matrix& mat)
    {
        mBufferData.MatVP = mat;
    }

private:
    SpaceCBuffer mBufferData;
};

