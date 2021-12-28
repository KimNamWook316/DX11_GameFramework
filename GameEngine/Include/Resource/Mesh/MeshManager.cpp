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
	// 2D에는 지금 메쉬가 하나만 필요하기 때문에, Init에서 생성한다.
	CMesh* spriteMesh = new CSpriteMesh;

	if (!spriteMesh->Init())
	{
		spriteMesh->Release();
		return false;
	}
	spriteMesh->SetName("SpriteMesh");

	// insert되면서, CSharePtr의 복사 생성자가 호출되기 때문에, 레퍼런스 카운트가 증가한다.
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
	frameRectMesh->SetName("frameRectMesh");
	mMapMesh.insert(std::make_pair("FrameRectMesh", frameRectMesh));

	// Collider 출력에 쓰일 메쉬
	CMesh* box2DMesh = new CStaticMesh;
	
	// 가운데 중점
	Vector3 box2DPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f),
	};

	box2DMesh->SetName("Box2DMesh");
	box2DMesh->CreateMesh(box2DPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mMapMesh.insert(std::make_pair("Box2DMesh", box2DMesh));
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