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
	frameRectMesh->SetName("FrameRectMesh");
	mMapMesh.insert(std::make_pair("FrameRectMesh", frameRectMesh));

	// Collider ��¿� ���� �޽�
	CMesh* box2DMesh = new CStaticMesh;
	
	// ��� ����
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

	// ProcedualMapGenerator�� �簢 �޽�
	CMesh* rectSpaceMesh = new CStaticMesh;
	Vector3 rectSpacePos[5] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};
	
	rectSpaceMesh->SetName("RectSpaceMesh");
	rectSpaceMesh->CreateMesh(rectSpacePos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mMapMesh.insert(std::make_pair("RectSpaceMesh", rectSpaceMesh));
	
	// �� �޽�
	CMesh* circleMesh = new CStaticMesh;
	Vector3 circlePos[31] = {};
	circlePos[0] = Vector3(0.5f, 0.f, 0.f);

	for (int i = 1; i < 31; ++i)
	{
		circlePos[i].x = cosf(DegToRad(12.f * i)) * 0.5f;
		circlePos[i].y = sinf(DegToRad(12.f * i)) * 0.5f;
	}

	circleMesh->SetName("CircleMesh");
	circleMesh->CreateMesh(circlePos, sizeof(Vector3), 31, D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	mMapMesh.insert(std::make_pair("CircleMesh", circleMesh));

	// UI �޽�
	CMesh* widgetMesh = new CStaticMesh;

	VertexUV widgetVtx[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f)),
	};

	int widgetIdx[6] = { 0, 1, 3, 0, 3, 2 };

	widgetMesh->SetName("WidgetMesh");
	widgetMesh->CreateMesh(widgetVtx, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, widgetIdx, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);

	mMapMesh.insert(std::make_pair("WidgetMesh", widgetMesh));

	// Particle�� Point Mesh
	CMesh* particlePointMesh = new CStaticMesh;
	Vector3 point;
	particlePointMesh->SetName("ParticlePointMesh");
	particlePointMesh->CreateMesh(&point, sizeof(Vector3), 1, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	mMapMesh.insert(std::make_pair("ParticlePointMesh", particlePointMesh));

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