#include "MaterialManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	// 기본 Material 생성
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
		// 자기 자신만이 가지고 있다면, 지운다.
		// CSharedPtr이 소멸되면서, Release()호출되며 RefCount 0이 된다.
		if (iter->second->GetRefCount() == 1)
		{
			mMapMaterial.erase(iter);
		}
	}
}
