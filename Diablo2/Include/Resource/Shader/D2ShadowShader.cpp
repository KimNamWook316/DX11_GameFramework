#include "D2ShadowShader.h"

CD2ShadowShader::CD2ShadowShader()
{
	SetTypeID<CD2ShadowShader>();
}

CD2ShadowShader::~CD2ShadowShader()
{
}

bool CD2ShadowShader::Init()
{
	if (!LoadVertexShader("D2ShadowVS", TEXT("D2ShadowShader.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	if (!LoadPixelShader("D2ShadowPS", TEXT("D2ShadowShader.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		assert(false);
		return false;
	}
	return true;
}
