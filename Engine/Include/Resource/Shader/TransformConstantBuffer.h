#pragma once
#include "ConstantBufferBase.h"
class CTransformConstantBuffer : public CConstantBufferBase
{
public:
    CTransformConstantBuffer();
    CTransformConstantBuffer(const CTransformConstantBuffer& buf);
    virtual ~CTransformConstantBuffer();

public:
    void SetWorldMatrix(const Matrix& mat)
    {
        mBufferData.MatWorld = mat;
    }

    void SetProjMatrix(const Matrix& mat)
    {
        mBufferData.MatProj = mat;
    }

    void SetMeshSize(const Vector3& size)
    {
        mBufferData.MeshSize = size;
    }

    void SetPivot(const Vector3& pivot)
    {
        mBufferData.Pivot = pivot;
    }

public:
    virtual bool Init();
    void UpdateCBuffer();
    CTransformConstantBuffer* Clone();

protected:
    TransformCBuffer mBufferData;
};

