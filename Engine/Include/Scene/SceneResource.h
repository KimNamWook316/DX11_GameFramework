#pragma once

#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"

// 纠喊 府家胶 包府 按眉
class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

public:
	CMesh* FindMesh(const std::string& name);

public:
	CShader* FindShader(const std::string& name);

	template <typename T>
	bool CreateShader(const std::string& name)
	{
		if (FindShader(name))
		{
			return false;
		}

		CResourceManager::GetInst()->CreateShader<T>(name);
		mMapShader.insert(std::make_pair(name, CResourceManager::GetInst()->FindShader(name)));

		return true;
	}

public:
	CMaterial* FindMaterial(const std::string& name);

	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		if (FindMaterial(name))
		{
			return false;
		}

		CResourceManager::GetInst()->CreateMaterial<T>(name);
		mMapMaterial.insert(std::make_pair(name, CResourceManager::GetInst()->FindMaterial(name)));

		return true;
	}
		

private:
	class CScene* mScene;
	std::unordered_map <std::string, CSharedPtr<CMesh>> mMapMesh;
	std::unordered_map <std::string, CSharedPtr<CShader>> mMapShader;
	std::unordered_map <std::string, CSharedPtr<CMaterial>> mMapMaterial;
};

