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
	class CTexture* FindTexture(const std::string& name);
	bool LoadTexture(const std::string& name, const TCHAR* fileName, 
		const std::string& path = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, 
		const std::string& path = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);
	void ReleaseTexture(const std::string& name);

public:
	bool CreateSampler(const std::string& name, D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE addressU,
		D3D11_TEXTURE_ADDRESS_MODE addressV,
		D3D11_TEXTURE_ADDRESS_MODE addressW,
		float boarderColor[4]);
	ID3D11SamplerState* FindSampler(const std::string& name);
	void SetSampler(const std::string& name, const int reg,
		const int shaderType = (int)eConstantBufferShaderTypeFlags::All);

private:
	std::unordered_map<std::string, CSharedPtr<class CTexture>> mMapTexture;
	std::unordered_map<std::string, ID3D11SamplerState*> mMapSampler;
};

