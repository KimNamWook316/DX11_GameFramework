#include "ResourceManager.h"
#include "SpriteMesh.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

// 2D���� ���� �޽��� �ϳ��� �ʿ��ϱ� ������, Init���� �����Ѵ�.
bool CResourceManager::Init()
{
	CMesh* spriteMesh = new CSpriteMesh;

	if (!spriteMesh->Init())
	{
		return false;
	}

	// insert�Ǹ鼭, CSharePtr�� ���� �����ڰ� ȣ��Ǳ� ������, ���۷��� ī��Ʈ�� �����Ѵ�.
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


