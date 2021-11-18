#include "MeshManager.h"
#include "SpriteMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	// 2D���� ���� �޽��� �ϳ��� �ʿ��ϱ� ������, Init���� �����Ѵ�.
	CMesh* spriteMesh = new CSpriteMesh;

	if (!spriteMesh->Init())
	{
		spriteMesh->Release();
		return false;
	}

	// insert�Ǹ鼭, CSharePtr�� ���� �����ڰ� ȣ��Ǳ� ������, ���۷��� ī��Ʈ�� �����Ѵ�.
	mMapMesh.insert(std::make_pair("SpriteMesh", spriteMesh));

	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& name)
{
	auto iter = mMapMesh.find(name);

	if (iter == mMapMesh.end())
	{
		return nullptr;
	}
	return iter->second;
}
