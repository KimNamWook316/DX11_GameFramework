#pragma once

#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"
#include "Texture/TextureManager.h"
#include "Animation/AnimationManager.h"

// �پ��� ���ҽ����� ��� ������ �Ŵ��� (�޽�, ����� ��� ��� ���ҽ��� �� ��ü������ �����Ѵ�.)
// CResource��� Ŭ������ �����, ��� ���ҽ��� ��ӹް� �Ͽ�, �� �ϳ��� �����ϴ� ���� �ִ�.
// Ȥ�� Hash�� ���� �����ϴ� ����� �ִµ�
// ���� ��� ������ �þ ��� ��� ���ҽ��� �ϳ��� �ʿ� ����ֱ� ������ Ž���� �������� ����
// Hash Table�� ��� Ž�� ��Ȯ���� ���� �ڷᰣ�� �Ÿ��� ũ�� ��ƾ� �ϱ� ������ �޸𸮸� ũ�� �����ϰ� �ȴ�.
// ���� ���⼭�� ��� ���ҽ��� ���� �����Ѵ�.
// �̰��� �� �����̶�� �� ���� ������, ���ҽ� ������ ���� ���� ����� �����ϰ� �ؾ� �Ѵ�.
class CResourceManager
{
public:
	bool Init();

public: // ===================== Mesh =====================
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);


public: // ===================== Shader =====================
	class CShader* FindShader(const std::string& name);
	void ReleaseShader(const std::string& name);

	template <typename T>
	bool CreateShader(const std::string& name)
	{
		return mShaderManager->CreateShader<T>(name);
	}

	bool CreateConstantBuffer(const std::string& name, const int size, const int reg,
		const int eConstantBufferShaderType = (int)eConstantBufferShaderTypeFlags::All);

	class CConstantBuffer* FindConstantBuffer(const std::string& name);

public: // ===================== Material =====================
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);

	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		return mMaterialManager->CreateMaterial<T>(name);
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		return mMaterialManager->CreateMaterialEmpty<T>();
	}

public: // ===================== Texture =====================
	class CTexture* FindTexture(const std::string& name);
	void ReleaseTexture(const std::string& name);

	bool LoadTexture(const std::string& name, const TCHAR* fileName, 
		const std::string& filePath = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);

public: // ===================== Sequence2D =====================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	void AddAnimationSequece2DFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddAnimationSequece2DFrame(const std::string& name, const float startX, const float startY,
		const float width, const float height);
	CAnimationSequence2D* FindAnimationSequece2D(const std::string& name);
	void ReleaseAnimationSequece2D(const std::string& name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const;
	bool SaveSequence2D(const std::string& name, const char* fullPath);
	bool LoadSequence2D(std::string& outName, const char* fullPath, class CScene* scene = nullptr);
	bool LoadSequence2D(const char* fullPath, class CScene* scene = nullptr);
	bool LoadSequence2D(std::string& outName, FILE* fp, class CScene* scene = nullptr);

	DECLARE_SINGLE(CResourceManager)
private:
	CMeshManager* mMeshManager;
	CShaderManager* mShaderManager;
	CMaterialManager* mMaterialManager;
	CTextureManager* mTextureManager;
	CAnimationManager* mAnimationManager;
};

