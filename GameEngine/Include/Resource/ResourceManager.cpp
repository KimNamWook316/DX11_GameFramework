#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
	: mMeshManager(nullptr)
	, mShaderManager(nullptr)
	, mMaterialManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mShaderManager);
	SAFE_DELETE(mMaterialManager);
}

bool CResourceManager::Init()
{
	mMeshManager = new CMeshManager;
	if (!mMeshManager->Init())
	{
		return false;
	}

	mShaderManager = new CShaderManager;
	if (!mShaderManager->Init())
	{
		return false;
	}

	mMaterialManager = new CMaterialManager;
	if (!mMaterialManager->Init())
	{
		return false;
	}

	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& name)
{	
	return mMeshManager->FindMesh(name);
}

void CResourceManager::ReleaseMesh(const std::string& name)
{
	return mMeshManager->ReleaseMesh(name);
}

CShader* CResourceManager::FindShader(const std::string& name)
{
	return mShaderManager->FindShader(name);
}

void CResourceManager::ReleaseShader(const std::string& name)
{
	return mShaderManager->ReleaseShader(name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& name)
{
	return mMaterialManager->FindMaterial(name);
}

void CResourceManager::ReleaseMaterial(const std::string& name)
{
	return mMaterialManager->ReleaseMaterial(name);
}


