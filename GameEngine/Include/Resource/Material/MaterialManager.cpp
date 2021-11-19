#include "MaterialManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	// �⺻ Material ����
	CreateMaterial<CMaterial>("Color");

	CSharedPtr<CMaterial> mtrl = FindMaterial("Color");

	mtrl->SetShader("ColorMeshShader");

	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& name)
{
	auto iter = mMapMaterial.find(name);

	if (iter == mMapMaterial.end())
	{
		return nullptr;
	}
	
	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& name)
{
	auto iter = mMapMaterial.find(name);

	if (iter != mMapMaterial.end())
	{
		// �ڱ� �ڽŸ��� ������ �ִٸ�, �����.
		// CSharedPtr�� �Ҹ�Ǹ鼭, Release()ȣ��Ǹ� RefCount 0�� �ȴ�.
		if (iter->second->GetRefCount() == 1)
		{
			mMapMaterial.erase(iter);
		}
	}
}
