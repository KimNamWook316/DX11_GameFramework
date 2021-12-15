#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	size_t size = mVecContainer.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecContainer[i]);
	}
}

bool CMesh::Init()
{
	return true;
}

// void* ���·� data�� �޾Ƽ�, ������ ���Ŀ� ���ֹ��� �ʰ� ���� �� �ִ�. ( �پ��� �迭 ���·� ������ )
bool CMesh::CreateBuffer(eBufferType eType, void* data, int size, 
	int count, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC desc = {};

	// �� ����Ʈ ũ��
	desc.ByteWidth = size * count;
	// Default, Immutable, dynamic, staging ( GPU, CPU ���� ���� �� ����/�б� ����)
	desc.Usage = usage;

	// ���ҽ��� ���������ο� ���ε��ϴ� ��� ����
	if (eBufferType::VERTEX == eType)
	{
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}

	// ���� �뵵 ���� ( cpu access, write/read ) 
	if (D3D11_USAGE_DYNAMIC == usage)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (D3D11_USAGE_STAGING == usage)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_SUBRESOURCE_DATA bufferData = {};

	// �ʱ�ȭ �����Ϳ� ���� ������ ����
	bufferData.pSysMem = data;

	// ���� ����
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, &bufferData, buffer)))
	{
		return false;
	}

	// Vertex Buffet�� ��� ��ġ ���� min, max ����
	if (eBufferType::VERTEX == eType)
	{
		// 1byte ������ ������ �����ؼ�, data�迭�� ���������� �����Ѵ�.
		char* vertexData = (char*)data;

		for (int i = 0; i < count; ++i)
		{
			// Vertex ���� ( ex : VertextColor ) ���� *** ��ġ ���� ���� !!! *** ����Ǿ� �����Ƿ�,
			// Vector3* �� ����ȯ�� �����ϸ�, ��ġ ������ �����Ѵ�.
			Vector3* pos = (Vector3*)vertexData;
			// �����ŭ ������ �����ϸ�, ���� data�� ���������� �����Ѵ�.
			vertexData += size;

			if (mMin.x > pos->x)
			{
				mMin.x = pos->x;
			}

			if (mMin.y > pos->y)
			{
				mMin.y = pos->y;
			}

			if (mMax.z < pos->z)
			{
				mMax.z = pos->z;
			}

			if (mMax.x < pos->x)
			{
				mMax.x = pos->x;
			}

			if (mMax.y < pos->y)
			{
				mMax.y = pos->y;
			}

			if (mMax.z < pos->z)
			{
				mMax.z = pos->z;
			}
		}
	}
	return true;
}

void CMesh::Render()
{
	size_t size = mVecContainer.size();

	for (size_t i = 0; i < size; ++i)
	{
		unsigned int	stride = mVecContainer[i]->VB.Size;	// ������ ���� �Ÿ� : Vertex ��ü�� ũ��
		unsigned int	offset = 0;							// ������

		// Input Assembler �ܰ�
		// Vertex �̾��ִ� ��� ����
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(mVecContainer[i]->Primitive);

		// VertexBuffer �Ѱ��ֱ�
		// ���� ���� Buffer�� �Ѱ��� �� �ִ�. ������ �ϳ�.
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&mVecContainer[i]->VB.Buffer, &stride, &offset);

		// �ε��� ������ ��
		size_t idxCount = mVecContainer[i]->vecIB.size();

		// �ε��� ���۰� �ϳ� �̻� ������ ��
		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; ++j)
			{
				// �ε��� ���� Context�� �Ѱ��ְ�
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					mVecContainer[i]->vecIB[j].Buffer,
					mVecContainer[i]->vecIB[j].Fmt, 0);

				// �ε��� ������ ���ҵ� ���� �ѱ��, �׸��� �����Ѵ�.
				CDevice::GetInst()->GetContext()->DrawIndexed(
					mVecContainer[i]->vecIB[j].Count, 0, 0);
			}
		}

		// �ε��� ���۰� ���� ��
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);

			// ������ Topology��, VetexBuffer������ �׸���.
			CDevice::GetInst()->GetContext()->Draw(
				mVecContainer[i]->VB.Count, 0);
		}
	}
}

bool CMesh::CreateMesh(void* vertexData, const int size, const int count, D3D11_USAGE usage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* indexData, const int indexSize, const int indexCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	MeshContainer* container = new MeshContainer;

	container->VB.Size = size;
	container->VB.Count = count;
	container->Primitive = primitive;

	if (!CreateBuffer(eBufferType::VERTEX, vertexData, size, count, usage,
		&container->VB.Buffer))
	{
		assert(false);
		return false;
	}

	if (indexData)
	{
		container->vecIB.resize(1);

		container->vecIB[0].Size = indexSize;
		container->vecIB[0].Count = indexCount;
		container->vecIB[0].Fmt = fmt;

		if (!CreateBuffer(eBufferType::INDEX, indexData, indexSize,
			indexCount, idxUsage, &container->vecIB[0].Buffer))
		{
			assert(false);
			return false;
		}
	}

	mVecContainer.push_back(container);
	return true;
}
