#pragma once
#include "../Ref.h"

class CMesh : public CRef
{
    friend class CResourceManager;
protected:
    CMesh();
    virtual ~CMesh() = 0;

public:
    virtual bool Init();

protected:
    bool CreateBuffer(Buffer_Type eType, void* data, int size, int count,
        D3D11_USAGE usage, ID3D11Buffer** buffer);

    virtual void Render();

protected:
    std::vector<MeshContainer> mVecContainer;

    Vector3 mMin;
    Vector3 mMax;
};

