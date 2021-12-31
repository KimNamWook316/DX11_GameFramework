#pragma once

#include "ConstantBufferBase.h"

// Transform cbuffer에 전해질 버퍼 객체
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

    void SetViewMatrix(const Matrix& mat)
    {
        mBufferData.MatView = mat;
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

