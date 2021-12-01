#pragma once

#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/ResourceManager.h"

// ���� ���ҽ� ���� ��ü
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
			return true;
		}

		if (!CResourceManager::GetInst()->CreateShader<T>(name))
		{
			return false;
		}

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
			return true;
		}

		if (!CResourceManager::GetInst()->CreateMaterial<T>(name))
		{
			return false;
		}

		mMapMaterial.insert(std::make_pair(name, CResourceManager::GetInst()->FindMaterial(name)));

		return true;
	}
	
public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);

	class CTexture* FindTexture(const std::string& name);

private:
	class CScene* mScene;
	std::unordered_map <std::string, CSharedPtr<CMesh>> mMapMesh;
	std::unordered_map <std::string, CSharedPtr<CShader>> mMapShader;
	std::unordered_map <std::string, CSharedPtr<CMaterial>> mMapMaterial;
	std::unordered_map <std::string, CSharedPtr<CTexture>> mMapTexture;
};

