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
	void ReleaseMesh(const std::string& name);

public:
	template <typename T>
	bool CreateMesh(const std::string& name)
	{
		T* mesh = (T*)FindMesh(name);

		if (mesh)
		{
			return false;
		}

		mesh = new T;

		if (!mesh->Init())
		{
			SAFE_RELEASE(mesh);
			return false;
		}

		mesh->SetName(name);
		mMapMesh.insert(std::make_pair(name, mesh));
		return true;
	}

private:
	std::unordered_map<std::string, CSharedPtr<class CMesh>> mMapMesh;
};

