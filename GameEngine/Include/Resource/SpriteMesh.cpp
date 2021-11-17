#include "SpriteMesh.h"

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
	container.VB.Count = 4; // 2D���� �簢�� �޽� ���� �ٸ� ���� ������ �ʿ伺�� ����. ������ �̹��� ���ű� ����

	// �� ������ �־�����, �װͳ��� �մ� ���
	// Trianlge Strip�� ���, Trail(����)���� �׸� �� ���� ����Ѵ�.
	container.Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexColor vtx[4] =
	{
		// ī�޶� ���� ���, �⺻������ ViewPort�� �� ������ -1, �� �������� 1, �Ʒ��� -1, ���� 1�� �����ȴ�.
		VertexColor(Vector3(-0.5f, 0.5f, 0.f), Vector4::Red),
		VertexColor(Vector3(0.5f, 0.5f, 0.f), Vector4::Green),
		VertexColor(Vector3(-0.5f, -0.5f, 0.f), Vector4::Blue),
		VertexColor(Vector3(0.5f, -0.5f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f)),
	};

	// Vertex Buffer ����
	if (!CreateBuffer(Buffer_Type::VERTEX, vtx, sizeof(VertexColor),
		4, D3D11_USAGE_IMMUTABLE, &container.VB.Buffer))
	{
		return false;
	}

	IndexBuffer idxBuffer;

	// ���� ��� �ϳ��� byte��. R16_UINT == 2byte
	idxBuffer.Size = 2;
	idxBuffer.Count = 6;
	// 2byte ũ��, �޽��� �ε��� ������ ���� ��� 4byte�� �Ҵ��ϴ� ��쵵 �ִ�.
	idxBuffer.Fmt = DXGI_FORMAT_R16_UINT; 

	// �׸��� ���� ����
	unsigned short idx[6] = { 0,1,3,0,3,2 };
	
	// 2byte ũ�� unsigned short �迭 �Ѱܼ�, ���� ����
	if (!CreateBuffer(Buffer_Type::INDEX, idx, sizeof(unsigned short),
		6, D3D11_USAGE_IMMUTABLE, &idxBuffer.Buffer))
	{
		return false;
	}

	container.vecIB.push_back(idxBuffer);

	mVecContainer.push_back(container);

	return true;
}
