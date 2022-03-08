#include "PathFindTileShader.h"

CPathFindTileShader::CPathFindTileShader()
{
	SetTypeID<CPathFindTileShader>();
}

CPathFindTileShader::~CPathFindTileShader()
{
}

bool CPathFindTileShader::Init()
{
	if (!LoadVertexShader("PathFindTileVS", TEXT("PathFind.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}
	if (!LoadGeometryShader("PathFindTileGS", TEXT("PathFind.fx"), SHADER_PATH))
	{
		assert(false);
		return false;
	}
	if (!LoadPixelShader("PathFindTilePS", TEXT("PathFind.fx"), SHADER_PATH))
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
