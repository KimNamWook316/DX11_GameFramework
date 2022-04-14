#include "D2ShadowMesh.h"

CD2ShadowMesh::CD2ShadowMesh()
{
	SetTypeID<CD2ShadowMesh>();
}

CD2ShadowMesh::~CD2ShadowMesh()
{
}

bool CD2ShadowMesh::Init()
{
	MeshContainer* container = new MeshContainer;

	container->VB.Size = sizeof(VertexUV);

	container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 왼쪽으로 30도 치우쳐진 평행사변형
	// y크기는 2/3
	VertexUV vtx[4] =
	{
		VertexUV(Vector3(-sqrtf(3.f) / 3.f, 0.6666f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f -(sqrtf(3.f) / 3.f), 0.6666f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f)),
	};

	// Vertex Buffer 생성
	if (!CreateBuffer(eBufferType::VERTEX, vtx, sizeof(VertexUV),
		4, D3D11_USAGE_IMMUTABLE, &container->VB.Buffer))
	{
		assert(false);
		return false;
	}

	container->vecIB.resize(1);

	container->vecIB[0].Size = 2;
	container->vecIB[0].Count = 6;
	container->vecIB[0].Fmt = DXGI_FORMAT_R16_UINT;

	// 그리기 순서 설정
	unsigned short idx[6] = { 0,1,3,0,3,2 };

	// 2byte 크기 unsigned short 배열 넘겨서, 버퍼 생성
	if (!CreateBuffer(eBufferType::INDEX, idx, sizeof(unsigned short),
		6, D3D11_USAGE_IMMUTABLE, &container->vecIB[0].Buffer))
	{
		assert(false);
		return false;
	}

	mVecContainer.push_back(container);
	return true;
}
