#include "SpriteMesh.h"

CSpriteMesh::CSpriteMesh()
{
	SetTypeID<CSpriteMesh>();
}

CSpriteMesh::~CSpriteMesh()
{
}

bool CSpriteMesh::Init()
{
	MeshContainer* container = new MeshContainer;

	container->VB.Size = sizeof(VertexUV);
	container->VB.Count = 4; // 2D에는 사각형 메쉬 말고 다른 것이 존재할 필요성이 없다. 어차피 이미지 띄울거기 때문

	// 세 정점이 주어지면, 그것끼리 잇는 방식
	// Trianlge Strip의 경우, Trail(궤적)등을 그릴 때 자주 사용한다.
	container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexUV vtx[4] =
	{
		// 카메라가 없는 경우, 기본적으로 ViewPort의 맨 왼쪽은 -1, 맨 오른쪽은 1, 아래는 -1, 위는 1로 설정된다.
		// Winding order 준수할 것.
		// 왼쪽 아래 점 현재 Pivot
		VertexUV(Vector3(0.f, 1.0f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
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

	// 버퍼 요소 하나의 byte수. R16_UINT == 2byte
	container->vecIB[0].Size = 2;
	container->vecIB[0].Count = 6;
	// 2byte 크기, 메쉬의 인덱스 개수가 많은 경우 4byte로 할당하는 경우도 있다.
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
