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
	int length = (int)shaderName.length();
	
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
			int length = (int)stateName.length();
			
			fwrite(&length, sizeof(int), 1, fp);
			fwrite(stateName.c_str(), sizeof(char), length, fp);
		}
	}

	int textureCount = (int)mVecTextureInfo.size();

	fwrite(&textureCount, sizeof(int), 1, fp);

	for (int i = 0; i < textureCount; ++i)
	{
		int length = (int)mVecTextureInfo[i].Name.length();
		
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
	char shaderName[256] = {};
	int length = 0;

	fread(&length, sizeof(int), 1, fp);
	fread(shaderName, sizeof(char), length, fp);

	mShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(shaderName);

	fread(&mBaseColor, sizeof(Vector4), 1, fp);
	fread(&mOpacity, sizeof(float), 1, fp);

	for (int i = 0; i < (int)eRenderStateType::MAX; ++i)
	{
		bool bStateEnable = false;
		fread(&bStateEnable, sizeof(bool), 1, fp);

		if (bStateEnable)
		{
			char stateName[256] = {};
			int length = 0;

			fread(&length, sizeof(int), 1, fp);
			fread(stateName, sizeof(char), length, fp);

			mRenderStateArray[i] = CRenderManager::GetInst()->FindRenderState(stateName);
		}
	}

	int textureCount = 0;
	fread(&textureCount, sizeof(int), 1, fp);

	for (int i = 0; i < textureCount; ++i)
	{
		mVecTextureInfo.push_back(MaterialTextureInfo());

		char textureName[256] = {};
		int length = 0;
		fread(&length, sizeof(int), 1, fp);
		fread(textureName, sizeof(char), length, fp);
		mVecTextureInfo[i].Name = textureName;
		
		fread(&mVecTextureInfo[i].SamplerType, sizeof(eSamplerType), 1, fp);
		fread(&mVecTextureInfo[i].Register, sizeof(int), 1, fp);
		fread(&mVecTextureInfo[i].ShaderType, sizeof(int), 1, fp);

		int texNameLength = 0;
		char texName[256] = {};
		fread(&texNameLength, sizeof(int), 1, fp);
		fread(texName, sizeof(char), texNameLength, fp);

		eImageType eType;
		fread(&eType, sizeof(eImageType), 1, fp);

		int infoCount = 0;
		fread(&infoCount, sizeof(int), 1, fp);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string	texPathName;

		for (int i = 0; i < infoCount; ++i)
		{
			int pathSize = 0;
			TCHAR fullPath[MAX_PATH] = {};

			fread(&pathSize, sizeof(int), 1, fp);
			fread(fullPath, sizeof(TCHAR), pathSize, fp);
			vecFullPath.push_back(fullPath);
			
			TCHAR fileName[MAX_PATH] = {};
			fread(&pathSize, sizeof(int), 1, fp);
			fread(&fileName, sizeof(TCHAR), pathSize, fp);
			vecFileName.push_back(fileName);

			char pathName[MAX_PATH] = {};
			fread(&pathSize, sizeof(int), 1, fp);
			fread(&pathName, sizeof(char), pathSize, fp);
			texPathName = pathName;
		}

		switch (eType)
		{
		case eImageType::Atlas:
			if (vecFileName.size() == 1)
			{
				if (mScene)
				{
					mScene->GetResource()->LoadTexture(textureName, vecFileName[0].c_str(), texPathName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(textureName, vecFileName[0].c_str(), texPathName);
				}
			}
			break;
		case eImageType::Frame:
			break;
		case eImageType::Array:
			break;
		default:
			assert(false);
			break;
		}

		if (mScene)
		{
			mVecTextureInfo[i].Texture = mScene->GetResource()->FindTexture(texName);
		}
		else
		{
			mVecTextureInfo[i].Texture = CResourceManager::GetInst()->FindTexture(texName);
		}
	}
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
