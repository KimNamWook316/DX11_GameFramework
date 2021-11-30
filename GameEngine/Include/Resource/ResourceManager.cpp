#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
	mMeshManager(nullptr), 
	mShaderManager(nullptr),
	mMaterialManager(nullptr),
	mTextureManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(mTextureManager);
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mShaderManager);
	SAFE_DELETE(mMaterialManager);
}

bool CResourceManager::Init()
{
	mMeshManager = new CMeshManager;
	if (!mMeshManager->Init())
	{
		assert(false);
		return false;
	}

	mShaderManager = new CShaderManager;
	if (!mShaderManager->Init())
	{
		assert(false);
		return false;
	}

	mTextureManager = new CTextureManager;
	if (!mTextureManager->Init())
	{
		assert(false);
		return false;
	}

	mMaterialManager = new CMaterialManager;
	if (!mMaterialManager->Init())
	{
		assert(false);
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

bool CResourceManager::CreateConstantBuffer(const std::string& name, const int size, const int reg, const int eConstantBufferShaderType)
{
	return mShaderManager->CreateConstantBuffer(name, size, reg, eConstantBufferShaderType);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& name)
{
	return mShaderManager->FindConstantBuffer(name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& name)
{
	return mMaterialManager->FindMaterial(name);
}

void CResourceManager::ReleaseMaterial(const std::string& name)
{
	return mMaterialManager->ReleaseMaterial(name);
}

CTexture* CResourceManager::FindTexture(const std::string& name)
{
	return mTextureManager->FindTexture(name);
}

void CResourceManager::ReleaseTexture(const std::string& name)
{
	mTextureManager->ReleaseTexture(name);
}

bool CResourceManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& filePath)
{
	return mTextureManager->LoadTexture(name, fileName, filePath);
}


