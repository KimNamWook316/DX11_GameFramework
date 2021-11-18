#pragma once

#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"

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

public:
	template <typename T>
	bool CreateShader(const std::string& name)
	{
		return mShaderManager->CreateShader<T>(name);
	}
	class CShader* FindShader(const std::string& name);

	DECLARE_SINGLE(CResourceManager)
private:
	CMeshManager* mMeshManager;
	CShaderManager* mShaderManager;
};

