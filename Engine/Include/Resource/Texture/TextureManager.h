#pragma once

#include "../../GameInfo.h"

class CTextureManager
{
	friend class CResourceManager;

private:
	CTextureManager();
	~CTextureManager();

public:
	bool Init();
	bool LoadTexture(const std::string& name, const TCHAR* fileName, 
		const std::string& path = TEXTURE_PATH);
	class CTexture* FindTexture(const std::string& name);
	void ReleaseTexture(const std::string& name);

private:
	std::unordered_map<std::string, CSharedPtr<class CTexture>> mMapTexture;
};

