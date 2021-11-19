#pragma once

#include "Material.h"

class CMaterialManager
{
	friend class CResourceManager;

private:
	CMaterialManager();
	~CMaterialManager();

public:
	bool Init();
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		T* material = (T*)FindMaterial(name);

		if (material)
		{
			return false;
		}

		material = new T;
		material->SetName(name);
		mMapMaterial.insert(std::make_pair(name, material));

		return true;
	}

private:
	std::unordered_map<std::string, CSharedPtr<CMaterial>> mMapMaterial;
};
