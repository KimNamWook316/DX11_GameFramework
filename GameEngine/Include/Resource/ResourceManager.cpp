#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
	: mMeshManager(nullptr)
	, mShaderManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mShaderManager);
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

	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& name)
{	
	return mMeshManager->FindMesh(name);
}

CShader* CResourceManager::FindShader(const std::string& name)
{
	return mShaderManager->FindShader(name);
}


