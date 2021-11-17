#include "ResourceManager.h"
#include "SpriteMesh.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

// 2D에는 지금 메쉬가 하나만 필요하기 때문에, Init에서 생성한다.
bool CResourceManager::Init()
{
	CMesh* spriteMesh = new CSpriteMesh;

	if (!spriteMesh->Init())
	{
		return false;
	}

	// insert되면서, CSharePtr의 복사 생성자가 호출되기 때문에, 레퍼런스 카운트가 증가한다.
	mMapMesh.insert(std::make_pair("SpriteMesh", spriteMesh));

	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& name)
{
	auto iter = mMapMesh.find(name);

	if (iter == mMapMesh.end())
	{
		return nullptr;
	}

	return iter->second;
}


