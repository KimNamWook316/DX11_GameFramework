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

// void* 형태로 data를 받아서, 데이터 형식에 구애받지 않고 받을 수 있다. ( 다양한 배열 형태로 전해짐 )
bool CMesh::CreateBuffer(eBufferType eType, void* data, int size, 
	int count, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC desc = {};

	// 총 바이트 크기
	desc.ByteWidth = size * count;
	// Default, Immutable, dynamic, staging ( GPU, CPU 접근 권한 및 쓰기/읽기 권한)
	desc.Usage = usage;

	// 리소스를 파이프라인에 바인딩하는 방법 설정
	if (eBufferType::VERTEX == eType)
	{
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}

	// 버퍼 용도 정의 ( cpu access, write/read ) 
	if (D3D11_USAGE_DYNAMIC == usage)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (D3D11_USAGE_STAGING == usage)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_SUBRESOURCE_DATA bufferData = {};

	// 초기화 데이터에 대한 포인터 전달
	bufferData.pSysMem = data;

	// 버퍼 생성
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, &bufferData, buffer)))
	{
		return false;
	}

	// Vertex Buffet인 경우 위치 정보 min, max 저장
	if (eBufferType::VERTEX == eType)
	{
		// 1byte 단위로 포인터 연산해서, data배열의 시작점에만 접근한다.
		char* vertexData = (char*)data;

		for (int i = 0; i < count; ++i)
		{
			// Vertex 정보 ( ex : VertextColor ) 에는 *** 위치 정보 먼저 !!! *** 저장되어 있으므로,
			// Vector3* 로 형변환해 접근하면, 위치 정보에 접근한다.
			Vector3* pos = (Vector3*)vertexData;
			// 사이즈만큼 포인터 연산하면, 다음 data의 시작점으로 점프한다.
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
		unsigned int	stride = mVecContainer[i]->VB.Size;	// 데이터 간의 거리 : Vertex 자체의 크기
		unsigned int	offset = 0;							// 시작점

		// Input Assembler 단계
		// Vertex 이어주는 방식 설정
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(mVecContainer[i]->Primitive);

		// VertexBuffer 넘겨주기
		// 여러 개의 Buffer를 넘겨줄 수 있다. 지금은 하나.
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,
			&mVecContainer[i]->VB.Buffer, &stride, &offset);

		// 인덱스 버퍼의 수
		size_t idxCount = mVecContainer[i]->vecIB.size();

		// 인덱스 버퍼가 하나 이상 존재할 때
		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; ++j)
			{
				// 인덱스 버퍼 Context에 넘겨주고
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					mVecContainer[i]->vecIB[j].Buffer,
					mVecContainer[i]->vecIB[j].Fmt, 0);

				// 인덱스 버퍼의 원소들 개수 넘기고, 그리기 지시한다.
				CDevice::GetInst()->GetContext()->DrawIndexed(
					mVecContainer[i]->vecIB[j].Count, 0, 0);
			}
		}

		// 인덱스 버퍼가 없을 때
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(
				nullptr, DXGI_FORMAT_UNKNOWN, 0);

			// 설정된 Topology로, VetexBuffer만으로 그린다.
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
