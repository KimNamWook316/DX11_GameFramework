#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "Mesh2DShader.h"
#include "PosMeshShader.h"
#include "ColliderShader.h"
#include "ConstantBuffer.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Init()
{
	// 2D�� �簢 �޽� ����
	if (!CreateShader<CColorMeshShader>("ColorMeshShader"))
	{
		assert(false);
		return false;
	}

	// �����Ϳ� ���̾������� �簢 �޽�
	if (!CreateShader<CPosMeshShader>("PosMeshShader"))
	{
		assert(false);
		return false;
	}
	
	// �ؽ��Ŀ� ���̴� ����
	if (!CreateShader<CMesh2DShader>("Mesh2DShader"))
	{
		assert(false);
		return false;
	}

	// �ݶ��̴��� ���̾������� ���̴�
	if (!CreateShader<CColliderShader>("ColliderShader"))
	{
		assert(false);
		return false;
	}

	// ��� ���� ����
	CreateConstantBuffer("TransformBuffer", sizeof(TransformCBuffer), 0,
		(int)eConstantBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("MaterialCbuffer", sizeof(MaterialCBuffer), 1,
		(int)eConstantBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("Standard2DCBuffer", sizeof(Standard2DCBuffer), 2,
		(int)eConstantBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("Animation2DCBuffer", sizeof(Animation2DCBuffer), 10,
		(int)eConstantBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("ColliderCBuffer", sizeof(ColliderCBuffer), 11,
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
