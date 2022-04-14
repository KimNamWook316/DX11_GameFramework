#pragma once

#include "Resource/Mesh/Mesh.h"

class CD2ShadowMesh :
    public CMesh
{
	friend class CMeshManager;

protected:
	CD2ShadowMesh();
	virtual ~CD2ShadowMesh();

public:
	virtual bool Init();
};

