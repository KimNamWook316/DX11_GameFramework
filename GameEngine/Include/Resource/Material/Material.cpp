#include "Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../Shader/MaterialConstantBuffer.h"

CMaterial::CMaterial()	:	
	mBaseColor(Vector4::White),
	mScene(nullptr),
	mCBuffer(nullptr)
{
	SetTypeID<CMaterial>();
}

CMaterial::CMaterial(const CMaterial& mat)
{
	*this = mat;

	mScene = nullptr;

	mRefCount = 0;
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetShader(const std::string& shaderName)
{
	mShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(shaderName);
}

void CMaterial::Reset()
{
	mBaseColor = Vector4::White;
	size_t size = mVecTextureInfo.size();
	
	for (size_t i = 0; i < size; ++i)
	{
		mVecTextureInfo[i].Texture->ResetShader(mVecTextureInfo[i].Register, mVecTextureInfo[i].ShaderType, 0);
	}
}

void CMaterial::Render()
{
	if (mShader)
	{
		mShader->SetShader();
	}

	if (mCBuffer)
	{
		mCBuffer->SetBaseColor(mBaseColor);

		mCBuffer->UpdateCBuffer();
	}

	size_t size = mVecTextureInfo.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecTextureInfo[i].Texture->SetShader(mVecTextureInfo[i].Register, mVecTextureInfo[i].ShaderType, 0);
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::SetBaseColor(const Vector4& color)
{
	mBaseColor = color;
}

void CMaterial::SetBaseColor(const float r, const float g, const float b, const float a)
{
	mBaseColor = Vector4(r, g, b, a);
}

void CMaterial::AddTexture(const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	mVecTextureInfo.push_back(MaterialTextureInfo());

	size_t idx = mVecTextureInfo.size() - 1;

	mVecTextureInfo[idx].Register = reg;
	mVecTextureInfo[idx].ShaderType = shaderType;
	mVecTextureInfo[idx].Name = name;
	mVecTextureInfo[idx].Texture = texture;
}

void CMaterial::AddTexture(const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	if (!mScene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return;
		}

		texture = CResourceManager::GetInst()->FindTexture(name);
	}

	else
	{
		if (!mScene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return;
		}
		texture = mScene->GetResource()->FindTexture(name);
	}

	mVecTextureInfo.push_back(MaterialTextureInfo());
	
	size_t idx = mVecTextureInfo.size() - 1;

	mVecTextureInfo[idx].Register = reg;
	mVecTextureInfo[idx].ShaderType = shaderType;
	mVecTextureInfo[idx].Name = name;
	mVecTextureInfo[idx].Texture = texture;
}

void CMaterial::AddTextureFullPath(const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{

}

void CMaterial::AddTexture(const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
}

void CMaterial::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, CTexture* texture)
{
	mVecTextureInfo[index].Register = reg;
	mVecTextureInfo[index].ShaderType = shaderType;
	mVecTextureInfo[index].Name = name;
	mVecTextureInfo[index].Texture = texture;
}

void CMaterial::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	if (!mScene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return;
		}

		texture = CResourceManager::GetInst()->FindTexture(name);
	}

	else
	{
		if (!mScene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return;
		}
		texture = mScene->GetResource()->FindTexture(name);
	}

	mVecTextureInfo[index].Register = reg;
	mVecTextureInfo[index].ShaderType = shaderType;
	mVecTextureInfo[index].Name = name;
	mVecTextureInfo[index].Texture = texture;
}

void CMaterial::SetTextureFullPath(const int index, const int reg, const int shaderType, const std::string& name, const TCHAR* fullPath)
{
}

void CMaterial::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
}
