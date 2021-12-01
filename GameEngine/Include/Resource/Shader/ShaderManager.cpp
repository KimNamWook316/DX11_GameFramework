#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "Mesh2DShader.h"
#include "ConstantBuffer.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Init()
{
	// 2D용 사각 메쉬 생성
	if (!CreateShader<CColorMeshShader>("ColorMeshShader"))
	{
		assert(false);
		return false;
	}

	// 텍스쳐용 쉐이더 생성
	if (!CreateShader<CMesh2DShader>("Mesh2DShader"))
	{
		assert(false);
		return false;
	}

	// 상수 버퍼 생성
	CreateConstantBuffer("TransformBuffer", sizeof(TransformCBuffer), 0,
		(int)eConstantBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("MaterialCbuffer", sizeof(MaterialCBuffer), 1,
		(int)eConstantBufferShaderTypeFlags::Graphic);
	
	return true;
}

CShader* CShaderManager::FindShader(const std::string& name)
{
	auto iter = mMapShader.find(name);

	if (iter == mMapShader.end())
	{
		return nullptr;
	}

	return iter->second;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& name)
{
	auto iter = mMapConstantBuffer.find(name);

	if (iter == mMapConstantBuffer.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& name)
{
	auto iter = mMapShader.find(name);

	if (iter != mMapShader.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMapShader.erase(iter);
		}
	}
}

bool CShaderManager::CreateConstantBuffer(const std::string& name, const int size, const int reg, const int eConstantBufferShaderType)
{
	CConstantBuffer* buf = FindConstantBuffer(name);

	if (buf)
	{
		return true;
	}
	
	buf = new CConstantBuffer;
	buf->SetName(name);

	if (!buf->Init(size, reg, eConstantBufferShaderType))
	{
		SAFE_RELEASE(buf);
		assert(false);
		return false;
	}

	mMapConstantBuffer.insert(std::make_pair(name, buf));
	return true;
}
