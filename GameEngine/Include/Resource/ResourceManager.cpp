#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
	mMeshManager(nullptr),
	mShaderManager(nullptr),
	mMaterialManager(nullptr),
	mTextureManager(nullptr),
	mAnimationManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(mAnimationManager);
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mShaderManager);
	SAFE_DELETE(mMaterialManager);
	SAFE_DELETE(mTextureManager);
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

	mAnimationManager = new CAnimationManager;
	if (!mAnimationManager->Init())
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

bool CResourceManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	return mTextureManager->LoadTextureFullPath(name, fullPath);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	return mAnimationManager->CreateAnimationSequence2D(name, textureName, fileName, pathName);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	return mAnimationManager->CreateAnimationSequence2D(name, texture);
}

void CResourceManager::AddAnimationSequece2DFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
	mAnimationManager->AddFrame(name, start, size);
}

void CResourceManager::AddAnimationSequece2DFrame(const std::string& name, const float startX, const float startY, const float width, const float height)
{
	mAnimationManager->AddFrame(name, startX, startY, width, height);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequece2D(const std::string& name)
{
	return mAnimationManager->FindSequence(name);
}

void CResourceManager::ReleaseAnimationSequece2D(const std::string& name)
{
	mAnimationManager->ReleaseSequence(name);
}

CAnimation2DConstantBuffer* CResourceManager::GetAnimation2DCBuffer() const
{
	return mAnimationManager->GetAnimation2DCBuffer();
}


