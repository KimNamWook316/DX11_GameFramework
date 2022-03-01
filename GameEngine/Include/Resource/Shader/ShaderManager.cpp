#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "Mesh2DShader.h"
#include "PosMeshShader.h"
#include "ColliderShader.h"
#include "ColliderPixelShader.h"
#include "WidgetShader.h"
#include "ProgressBarShader.h"
#include "NumberShader.h"
#include "ParticleUpdateShader.h"
#include "ParticleRenderShader.h"
#include "SpaceRectShader.h"
#include "SpaceRhombusShader.h"
#include "TileMapShader.h"
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

	// 에디터용 와이어프레임 사각 메쉬
	if (!CreateShader<CPosMeshShader>("PosMeshShader"))
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

	// 콜라이더용 와이어프레임 쉐이더
	if (!CreateShader<CColliderShader>("ColliderShader"))
	{
		assert(false);
		return false;
	}

	// 픽셀 충돌체용 쉐이더
	if (!CreateShader<CColliderPixelShader>("ColliderPixelShader"))
	{
		assert(false);
		return false;
	}

	// UI 출력용 쉐이더
	if (!CreateShader<CWidgetShader>("WidgetShader"))
	{
		assert(false);
		return false;
	}
	
	// Progress Bar 쉐이더
	if (!CreateShader<CProgressBarShader>("ProgressBarShader"))
	{
		assert(false);
		return false;
	}

	// Number Widget 쉐이더
	if (!CreateShader<CNumberShader>("NumberShader"))
	{
		assert(false);
		return false;
	}

	// Particle Update Compute 쉐이더
	if (!CreateShader<CParticleUpdateShader>("ParticleUpdateShader"))
	{
		assert(false);
		return false;
	}

	// Particle Render Shader
	if (!CreateShader<CParticleRenderShader>("ParticleRenderShader"))
	{
		assert(false);
		return false;
	}

	// TileMap Shader
	if (!CreateShader<CTileMapShader>("TileMapShader"))
	{
		assert(false);
		return false;
	}

	// Space Shader
	if (!CreateShader<CSpaceRectShader>("SpaceRectShader"))
	{
		assert(false);
		return false;
	}

	if (!CreateShader<CSpaceRhombusShader>("SpaceRhombusShader"))
	{
		assert(false);
		return false;
	}

	// 상수 버퍼 생성
	CreateConstantBuffer("TransformBuffer", sizeof(TransformCBuffer), 0,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("MaterialCbuffer", sizeof(MaterialCBuffer), 1,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("Standard2DCBuffer", sizeof(Standard2DCBuffer), 2,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("Animation2DCBuffer", sizeof(Animation2DCBuffer), 10,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("ColliderCBuffer", sizeof(ColliderCBuffer), 11,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("WidgetCBuffer", sizeof(WidgetCBuffer), 11,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("ProgressBarCBuffer", sizeof(ProgressBarCBuffer), 12,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("GlobalCBuffer", sizeof(GlobalCBuffer), 3,
		(int)eBufferShaderTypeFlags::All);
	
	CreateConstantBuffer("ParticleCBuffer", sizeof(ParticleCBuffer), 11,
		(int)eBufferShaderTypeFlags::Compute);

	CreateConstantBuffer("DissolveCBuffer", sizeof(DissolveCBuffer), 4,
		(int)eBufferShaderTypeFlags::Pixel);

	CreateConstantBuffer("TileMapCBuffer", sizeof(TileMapCBuffer), 11,
		(int)eBufferShaderTypeFlags::Graphic);

	CreateConstantBuffer("SpaceCBuffer", sizeof(SpaceCBuffer), 11,
		(int)eBufferShaderTypeFlags::Graphic);

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
