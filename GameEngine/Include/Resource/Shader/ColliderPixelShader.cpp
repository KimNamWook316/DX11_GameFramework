#include "ColliderPixelShader.h"

CColliderPixelShader::CColliderPixelShader()
{
	SetTypeID<CColliderPixelShader>();
}

CColliderPixelShader::~CColliderPixelShader()
{
}

bool CColliderPixelShader::Init()
{
	if (!LoadVertexShader("ColliderPixelVS", TEXT("Collider.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	if (!LoadPixelShader("ColliderPixelPS", TEXT("Collider.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		assert(false);
		return false;
	}

	return true;
}
