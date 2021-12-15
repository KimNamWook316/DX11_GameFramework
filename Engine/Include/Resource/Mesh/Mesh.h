#pragma once
#include "../../Ref.h"

class CMesh : public CRef
{
    friend class CMeshManager;
protected:
    CMesh();
    virtual ~CMesh() = 0;

public:
    virtual bool Init();
    virtual void Render();

    virtual bool CreateMesh(void* vertexData, const int size, const int count, D3D11_USAGE usage,
        D3D11_PRIMITIVE_TOPOLOGY primitive, void* indetData = nullptr, const int indexSize = 0,
        const int indexCount = 0, D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);

protected:
    bool CreateBuffer(eBufferType eType, void* data, int size, int count,
        D3D11_USAGE usage, ID3D11Buffer** buffer);

protected:
    std::vector<MeshContainer*> mVecContainer;

    Vector3 mMin;
    Vector3 mMax;
};

