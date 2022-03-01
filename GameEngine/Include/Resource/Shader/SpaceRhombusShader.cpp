#include "SpaceRhombusShader.h"

CSpaceRhombusShader::CSpaceRhombusShader()
{
	SetTypeID<CSpaceRhombusShader>();
}

CSpaceRhombusShader::~CSpaceRhombusShader()
{
}

bool CSpaceRhombusShader::Init()
{
	if (!LoadVertexShader("SpaceRhombusVS", TEXT("Space.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}
	if (!LoadGeometryShader("SpaceGS", TEXT("Space.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}
	if (!LoadPixelShader("SpaceRhombusPS", TEXT("Space.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		assert(false);
		return false;
	}

	return true;
}
