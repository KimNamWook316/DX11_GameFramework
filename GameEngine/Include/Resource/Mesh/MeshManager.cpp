#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"

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
	spriteMesh->SetName("SpriteMesh");

	// insert�Ǹ鼭, CSharePtr�� ���� �����ڰ� ȣ��Ǳ� ������, ���۷��� ī��Ʈ�� �����Ѵ�.
	mMapMesh.insert(std::make_pair("SpriteMesh", spriteMesh));

	CMesh* frameRectMesh = new CStaticMesh;

	Vector3 frameRectPos[5] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};

	frameRectMesh->CreateMesh(frameRectPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mMapMesh.insert(std::make_pair("FrameRectMesh", frameRectMesh));
	frameRectMesh->SetName("frameRectMesh");

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

void CMeshManager::ReleaseMesh(const std::string& name)
{
	auto iter = mMapMesh.find(name);

	if (iter != mMapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMapMesh.erase(iter);
		}
	}
}