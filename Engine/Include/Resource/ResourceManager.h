#pragma once

#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"

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

	DECLARE_SINGLE(CResourceManager)
private:
	CMeshManager* mMeshManager;
	CShaderManager* mShaderManager;
	CMaterialManager* mMaterialManager;
};

