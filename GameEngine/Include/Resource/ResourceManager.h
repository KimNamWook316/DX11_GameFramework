#pragma once

#include "../GameInfo.h"

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

	DECLARE_SINGLE(CResourceManager)
private:
	std::unordered_map<std::string, CSharedPtr<class CMesh>> mMapMesh;
};

