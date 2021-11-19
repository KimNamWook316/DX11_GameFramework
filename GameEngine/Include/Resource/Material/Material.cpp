#include "Material.h"
#include "../ResourceManager.h"

CMaterial::CMaterial()
{
	SetTypeID<CMaterial>();
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetShader(const std::string& shaderName)
{
	mShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(shaderName);
}

void CMaterial::Render()
{
	if (mShader)
	{
		mShader->SetShader();
	}
}
