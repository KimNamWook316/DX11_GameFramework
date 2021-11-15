#include "CSpriteMesh.h"

CSpriteMesh::CSpriteMesh()
{
}

CSpriteMesh::~CSpriteMesh()
{
}

bool CSpriteMesh::Init()
{
	MeshContainer container;

	container.VB.Size = sizeof(VertexColor);
	container.VB.Count = 4; // 2D에는 사각형 메쉬 말고 다른 것이 존재할 필요성이 없다. 어차피 이미지 띄울거기 때문

	// 세 정점이 주어지면, 그것끼리 잇는 방식
	// Trianlge Strip의 경우, Trail(궤적)등을 그릴 때 자주 사용한다.
	container.Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexColor vtx[4] =
	{
		// 카메라가 없는 경우, 기본적으로 ViewPort의 맨 왼쪽은 -1, 맨 오른쪽은 1, 아래는 -1, 위는 1로 설정된다.
		VertexColor(Vector3(-0.5f, 0.5f, 0.f), Vector4::Red),
		VertexColor(Vector3(0.5f, 0.5f, 0.f), Vector4::Red),
		VertexColor(Vector3(-0.5f, -0.5f, 0.f), Vector4::Red),
		VertexColor(Vector3(0.5f, -0.5f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f)),
	};

	IndexBuffer idxBuffer;

	// 삼각형 두 개로 사각형을 만들 것이기 때문
	idxBuffer.Size = 2;
	idxBuffer.Count = 6;
	idxBuffer.Fmt = DXGI_FORMAT_R16_UINT; // 2byte 크기, 4byte형으로 하면 억단위의 인덱스를 저장할 수 있지만, 흔치 않다.

	// 그리기 순서 설정
	unsigned short idx[6] = { 0,1,3,0,3,2 };
	
	// 2byte 크기 unsigned short 배열 넘겨서, 버퍼 생성
	if (!CreateBuffer(Buffer_Type::INDEX, idx, sizeof(unsigned short),
		6, D3D11_USAGE_IMMUTABLE, &idxBuffer.Buffer))
	{
		return false;
	}

	container.vecIB.push_back(idxBuffer);
	return true;
}
