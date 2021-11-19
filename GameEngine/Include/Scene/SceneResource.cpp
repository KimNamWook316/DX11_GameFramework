#include "SceneResource.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto iter = mMapMesh.begin();
		auto iterEnd = mMapMesh.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			// erase하면 다음 iterator 반환하며 반복
			// refCount--
			iter = mMapMesh.erase(iter);

			CResourceManager::GetInst()->ReleaseMesh(name);
		}
	}

	{
		auto iter = mMapShader.begin();
		auto iterEnd = mMapShader.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			iter = mMapShader.erase(iter);

			CResourceManager::GetInst()->ReleaseShader(name);
		}
	}

	{
		auto iter = mMapMaterial.begin();
		auto iterEnd = mMapMaterial.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			iter = mMapMaterial.erase(iter);

			CResourceManager::GetInst()->ReleaseMaterial(name);
		}
	}
}

CMesh* CSceneResource::FindMesh(const std::string& name)
{
	auto iter = mMapMesh.find(name);

	if (iter == mMapMesh.end())
	{
		return nullptr;
	}

	return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& name)
{
	auto iter = mMapShader.find(name);

	if (iter == mMapShader.end())
	{
		return nullptr;
	}

	return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& name)
{
	auto iter = mMapMaterial.find(name);

	if (iter == mMapMaterial.end())
	{
		return nullptr;
	}

	return iter->second;
}
