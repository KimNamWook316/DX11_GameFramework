#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"
#include "../Shader/MaterialConstantBuffer.h"

// Material이 들고 있는 텍스쳐와 정보
struct MaterialTextureInfo
{
	std::string Name;
	
	// 샘플링 타입
	eSamplerType SamplerType;
	CSharedPtr<CTexture> Texture;
	int Register;
	int ShaderType;

	MaterialTextureInfo() :
		SamplerType(eSamplerType::Linear),
		Register(0),
		ShaderType((int)eBufferShaderTypeFlags::Pixel)
	{
	}
};

struct RenderCallBack
{
	std::function<void()> Func;
	void* Obj;
};

class CMaterial : public CRef
{
	friend class CMaterialManager;

protected:
	CMaterial();
	CMaterial(const CMaterial& mat);
	virtual ~CMaterial();

public:
	void SetShader(const std::string& shaderName);
	void Reset();
	void Render();
	CMaterial* Clone();

public:
	void Save(FILE* fp);
	void Load(FILE* fp);

public:
	void SetScene(class CScene* scene)
	{
		mScene = scene;
	}

public:
	void SetRenderState(class CRenderState* state);
	void SetRenderState(const std::string& name);
	void SetTransparency(bool bEnable);
	void SetOpacity(const float val);
	void AddOpacity(const float val);
	void SetDissolve(bool bEnable);

public:
	void SetBaseColor(const Vector4& color);
	void SetBaseColor(const float r, const float g, const float b, const float a);

public:
	void AddTexture(const int reg, const int shaderType, const std::string& name,
		class CTexture* texture);
	void AddTexture(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddTextureFullPath(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fullPath);
	void AddTexture(const int reg, const int shaderType, const std::string& name,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	
	void SetTexture(const int index, const int reg, const int shaderType,
		const std::string& name, class CTexture* texture);
	void SetTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(const int index, const int reg, const int shaderType, 
		const std::string& name, const TCHAR* fullPath);
	void SetTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const std::vector<TCHAR*>& vecFileName, 
		const std::string& pathName = TEXTURE_PATH);
	
public:
	CTexture* GetTexture(const int textureIdx = 0) const
	{
		return mVecTextureInfo[textureIdx].Texture;
	}

	unsigned int GetTextureWidth(const int textureIdx = 0) const
	{
		return mVecTextureInfo[textureIdx].Texture->GetWidth();
	}

	unsigned int GetTextureHeight(const int textureIdx = 0) const
	{
		return mVecTextureInfo[textureIdx].Texture->GetHeight();
	}

	const Vector4& GetBaseColor() const
	{
		return mBaseColor;
	}

	float GetOpacity() const
	{
		return mOpacity;
	}

	bool GetIsTransparentActive() const
	{
		if (mRenderStateArray[(int)eRenderStateType::Blend])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsTextureEmpty() const
	{
		return mVecTextureInfo.empty();
	}

public:
	template <typename T>
	void AddRenderCallBack(T* obj, void(T::* func)())
	{
		RenderCallBack* callBack = new RenderCallBack;
		callBack->Obj = obj;
		callBack->Func = std::bind(func, obj);
		mRenderCallBackList.push_back(callBack);
	}

	void DeleteRenderCallBack(void* obj)
	{
		auto iter = mRenderCallBackList.begin();
		auto iterEnd = mRenderCallBackList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (obj == (*iter)->Obj)
			{
				SAFE_DELETE(*iter);
				mRenderCallBackList.erase(iter);
				break;
			}
		}
	}

private:
	void SetConstantBuffer(class CMaterialConstantBuffer* buffer)
	{
		mCBuffer = buffer->Clone();
	}

protected:
	CSharedPtr<CGraphicShader> mShader;
	std::vector<MaterialTextureInfo> mVecTextureInfo;

	Vector4 mBaseColor;
	float mOpacity;

	// BaseColor등을 쉐이더로 보내는 용도의 상수버퍼 
	// 단지 데이터를 쓰기 위한 것이므로 공유해도 되기 때문에, 
	// MaterialManager에서 하나만 가지고 있다.
	class CMaterialConstantBuffer* mCBuffer;
	CSharedPtr<class CRenderState> mRenderStateArray[(int)eRenderStateType::MAX];
	std::list<RenderCallBack*> mRenderCallBackList;

private:
	class CScene* mScene;
};

