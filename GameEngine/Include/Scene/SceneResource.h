#pragma once

#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/ResourceManager.h"

// 纠喊 府家胶 包府 按眉
class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

public:
	CMesh* FindMesh(const std::string& name);

public:
	CShader* FindShader(const std::string& name);

	template <typename T>
	bool CreateShader(const std::string& name)
	{
		if (FindShader(name))
		{
			return true;
		}

		if (!CResourceManager::GetInst()->CreateShader<T>(name))
		{
			return false;
		}

		mMapShader.insert(std::make_pair(name, CResourceManager::GetInst()->FindShader(name)));

		return true;
	}

public:
	CMaterial* FindMaterial(const std::string& name);

	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		if (FindMaterial(name))
		{
			return true;
		}

		if (!CResourceManager::GetInst()->CreateMaterial<T>(name))
		{
			return false;
		}

		mMapMaterial.insert(std::make_pair(name, CResourceManager::GetInst()->FindMaterial(name)));

		return true;
	}
	
public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	class CTexture* FindTexture(const std::string& name);

public:
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	void AddAnimationSequece2DFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddAnimationSequece2DFrame(const std::string& name, const float startX, const float startY,
		const float width, const float height);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const;
	void ReleaseAnimationSequence2D(const std::string& name);

public:
	bool SaveSequence2D(const std::string& name, const char* fullPath);
	bool LoadSequence2D(const char* fullPath);
	bool LoadSequence2D(std::string& outName, const char* fullPath);
	bool LoadSequence2D(std::string& outName, FILE* fp);

private:
	class CScene* mScene;
	std::unordered_map <std::string, CSharedPtr<CMesh>> mMapMesh;
	std::unordered_map <std::string, CSharedPtr<CShader>> mMapShader;
	std::unordered_map <std::string, CSharedPtr<CMaterial>> mMapMaterial;
	std::unordered_map <std::string, CSharedPtr<CTexture>> mMapTexture;
	std::unordered_map <std::string, CSharedPtr<CAnimationSequence2D>> mMapSequence2D;
};

