#include "TextureManager.h"
#include "Texture.h"
#include "../../Device.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
	auto iter = mMapSampler.begin();
	auto iterEnd = mMapSampler.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

bool CTextureManager::Init()
{
	// Default Texture
	LoadTexture("EngineDefaultTexture", TEXT("teemo.png"));

	// Default Sampler State
	float boarderColor[4] = {};

	boarderColor[0] = 1.f;
	boarderColor[1] = 1.f;
	boarderColor[2] = 1.f;
	boarderColor[3] = 1.f;

	if (!CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		boarderColor))
	{
		assert(false);
		return false;
	}

	if (!CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		boarderColor))
	{
		assert(false);
		return false;
	}

	if (!CreateSampler("Anisotropic", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		boarderColor))
	{
		assert(false);
		return false;
	}

	SetSampler("Point", 0);
	SetSampler("Linear", 1);
	SetSampler("Anisotropic", 2);

	SetSampler("Linear", 3);

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

bool CTextureManager::CreateSampler(const std::string& name, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV, D3D11_TEXTURE_ADDRESS_MODE addressW, float boarderColor[4])
{
	ID3D11SamplerState* sampler = FindSampler(name);

	if (sampler)
	{
		return true;
	}

	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = filter;					// Filtering ��� -> point, linear, anisotropic
	desc.AddressU = addressU;				// [0,1] ������ �Ѿ �ؽ��� ��ǥ�� ���� ó�����
	desc.AddressV = addressV;
	desc.AddressW = addressW;
	desc.BorderColor[0] = boarderColor[0];	// ��輱 �÷�
	desc.BorderColor[1] = boarderColor[1];
	desc.BorderColor[2] = boarderColor[2];
	desc.BorderColor[3] = boarderColor[3];

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&desc, &sampler)))
	{
		assert(false);
		return false;
	}

	mMapSampler.insert(std::make_pair(name, sampler));

	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& name)
{
	auto iter = mMapSampler.find(name);
	
	if (iter == mMapSampler.end())
	{
		return nullptr;
	}

	return iter->second;
}

// ShaderInfo.fx �� ���ǵ� SamplerState�� ����(reg)�� SamplerState�� �����Ѵ�.
void CTextureManager::SetSampler(const std::string& name, const int reg, const int shaderType)
{
	ID3D11SamplerState* sampler = FindSampler(name);

	if (!sampler)
	{
		return;
	}

	if (shaderType & (int)eConstantBufferShaderTypeFlags::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetSamplers(reg, 1, &sampler);
	}
	if (shaderType & (int)eConstantBufferShaderTypeFlags::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetSamplers(reg, 1, &sampler);
	}
	if (shaderType & (int)eConstantBufferShaderTypeFlags::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetSamplers(reg, 1, &sampler);
	}
	if (shaderType & (int)eConstantBufferShaderTypeFlags::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetSamplers(reg, 1, &sampler);
	}
	if (shaderType & (int)eConstantBufferShaderTypeFlags::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetSamplers(reg, 1, &sampler);
	}
	if (shaderType & (int)eConstantBufferShaderTypeFlags::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetSamplers(reg, 1, &sampler);
	}
}
