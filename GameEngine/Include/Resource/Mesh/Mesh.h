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

protected:
    bool CreateBuffer(eBufferType eType, void* data, int size, int count,
        D3D11_USAGE usage, ID3D11Buffer** buffer);


protected:
    std::vector<MeshContainer*> mVecContainer;

    Vector3 mMin;
    Vector3 mMax;
};

