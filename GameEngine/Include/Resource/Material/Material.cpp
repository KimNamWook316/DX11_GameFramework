#include "Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../Shader/MaterialConstantBuffer.h"
#include "../../Render/RenderManager.h"
#include "../../Render/RenderState.h"

CMaterial::CMaterial()	:	
	mBaseColor(Vector4::White),
	mScene(nullptr),
	mCBuffer(nullptr),
	mOpacity(1.f),
	mRenderStateArray{}
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
	size_t size = mVecTextureInfo.size();
	
	for (size_t i = 0; i < size; ++i)
	{
		mVecTextureInfo[i].Texture->ResetShader(mVecTextureInfo[i].Register, mVecTextureInfo[i].ShaderType, 0);
	}

	for (int i = 0; i < (int)eRenderStateType::MAX; ++i)
	{
		if (mRenderStateArray[i])
		{
			mRenderStateArray[i]->ResetState();
		}
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
		mCBuffer->SetOpacity(mOpacity);

		mCBuffer->UpdateCBuffer();
	}

	for (int i = 0; i < (int)eRenderStateType::MAX; ++i)
	{
		if (mRenderStateArray[i])
		{
			mRenderStateArray[i]->SetState();
		}
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

void CMaterial::Save(FILE* fp)
{
	std::string shaderName = mShader->GetName();
	int length = (size_t)shaderName.length();
	
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(shaderName.c_str(), sizeof(char), length, fp);

	fwrite(&mBaseColor, sizeof(Vector4), 1, fp);
	fwrite(&mOpacity, sizeof(float), 1, fp);

	for (int i = 0; i < (int)eRenderStateType::MAX; ++i)
	{
		bool bStateEnable = false;

		if (mRenderStateArray[i])
		{
			bStateEnable = true;
		}

		fwrite(&bStateEnable, sizeof(bool), 1, fp);

		if (bStateEnable)
		{
			std::string stateName = mShader->GetName();
			int length = (size_t)stateName.length();
			
			fwrite(&length, sizeof(int), 1, fp);
			fwrite(stateName.c_str(), sizeof(char), length, fp);
		}
	}

	int textureCount = (int)mVecTextureInfo.size();

	fwrite(&textureCount, sizeof(int), 1, fp);

	for (int i = 0; i < textureCount; ++i)
	{
		int length = (size_t)mVecTextureInfo[i].Name.length();
		
		fwrite(&length, sizeof(int), 1, fp);
		fwrite(mVecTextureInfo[i].Name.c_str(), sizeof(char), length, fp);
		
		fwrite(&mVecTextureInfo[i].SamplerType, sizeof(eSamplerType), 1, fp);
		fwrite(&mVecTextureInfo[i].Register, sizeof(int), 1, fp);
		fwrite(&mVecTextureInfo[i].ShaderType, sizeof(eShaderType), 1, fp);

		mVecTextureInfo[i].Texture->Save(fp);
	}
}

void CMaterial::Load(FILE* fp)
{
}

void CMaterial::SetRenderState(CRenderState* state)
{
	mRenderStateArray[(int)eRenderStateType::Blend] = state;
}

void CMaterial::SetRenderState(const std::string& name)
{
	mRenderStateArray[(int)eRenderStateType::Blend] = CRenderManager::GetInst()->FindRenderState(name);
}

void CMaterial::SetTransparency(bool bEnable)
{
	if (bEnable)
	{
		mRenderStateArray[(int)eRenderStateType::Blend] = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
	}
}

void CMaterial::SetOpacity(const float val)
{
	mOpacity = val;

	if (mOpacity < 0)
	{
		mOpacity = 0.f;
	}
	else if (mOpacity > 1.f)
	{
		mOpacity = 1.f;
	}
}

void CMaterial::AddOpacity(const float val)
{
	mOpacity += val;

	if (mOpacity < 0)
	{
		mOpacity = 0.f;
	}
	else if (mOpacity > 1.f)
	{
		mOpacity = 1.f;
	}
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
	if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
	{
		return;
	}

	mVecTextureInfo[index].Register = reg;
	mVecTextureInfo[index].Name = name;
	mVecTextureInfo[index].Texture = CResourceManager::GetInst()->FindTexture(name);
	mVecTextureInfo[index].ShaderType = shaderType;
}

void CMaterial::SetTexture(const int index, const int reg, const int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
}
