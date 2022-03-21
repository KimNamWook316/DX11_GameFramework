#include "GridShader.h"

CGridShader::CGridShader()
{
	SetTypeID<CGridShader>();
}

CGridShader::~CGridShader()
{
}

bool CGridShader::Init()
{
	if (!LoadVertexShader("GridVS", TEXT("Widget.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	if (!LoadPixelShader("GridPS", TEXT("Widget.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		assert(false);
		return false;
	}

	return true;
}
