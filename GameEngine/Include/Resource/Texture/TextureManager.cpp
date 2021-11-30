#include "TextureManager.h"
#include "Texture.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

bool CTextureManager::Init()
{
	// Default Texture
	LoadTexture("EngineDefaultTexture", TEXT("teemo.png"));

	return true;
}

bool CTextureManager::LoadTexture(const std::string& name, const TCHAR* fileName, 
	const std::string& path)
{
	CTexture* texture = FindTexture(name);

	if (texture)
	{
		return true;
	}

	texture = new CTexture;

	if (!texture->LoadTexture(name, fileName, path))
	{
		SAFE_DELETE(texture);
		return false;
	}

	mMapTexture.insert(std::make_pair(name, texture));

	return true;
}

CTexture* CTextureManager::FindTexture(const std::string& name)
{
	auto iter = mMapTexture.find(name);

	if (iter == mMapTexture.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CTextureManager::ReleaseTexture(const std::string& name)
{
	auto iter = mMapTexture.find(name);

	if (iter != mMapTexture.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMapTexture.erase(iter);
		}
	}
}
