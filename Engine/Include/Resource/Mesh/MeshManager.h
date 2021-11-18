#pragma once

#include "../../GameInfo.h"

class CMeshManager
{
	friend class CResourceManager;

private:
	CMeshManager();
	~CMeshManager();

public:
	bool Init();
	class CMesh* FindMesh(const std::string& name);

private:
	std::unordered_map<std::string, CSharedPtr<class CMesh>> mMapMesh;
};

