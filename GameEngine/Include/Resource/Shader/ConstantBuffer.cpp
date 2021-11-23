#include "ConstantBuffer.h"
#include "../../Device.h"

CConstantBuffer::CConstantBuffer()	:
	mBuffer(nullptr),
	mSize(0),
	mRegister(0),
	meConstantBufferShaderType(0)
{
}

CConstantBuffer::~CConstantBuffer()
{
	SAFE_RELEASE(mBuffer);
}

bool CConstantBuffer::Init(const int& size, const int& reg, int eConstantBufferType)
{
	mSize = size;
	mRegister = reg;
	// 어떤 쉐이더에 공급될 용도인지 판단하는 Enum 값임
	meConstantBufferShaderType = eConstantBufferType;

	D3D11_BUFFER_DESC desc = {};
	
	desc.Usage = D3D11_USAGE_DYNAMIC;	// GPU가 읽고, CPU가 쓴다.
	desc.ByteWidth = size;				// 버퍼의 크기
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 상수 버퍼로 바인딩
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU에서 값을 전해줘야 하므로

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, &mBuffer))) 
	{
		assert(false);
		return false;
	}

	return true;
}

void CConstantBuffer::UpdateBuffer(void* data)
{
	D3D11_MAPPED_SUBRESOURCE map = {};

	// mBuffer 서브리소스에 대한 포인터(map 변수에)를 얻어오고, GPU Access를 제한한다.
	// CPU가 이제부터 쓰기 시작하겠다. 즉, buffer의 usage가 DYNAMIC일 때 가능
	// D3D11_MAP_WRITE_DISCARD : 기존 데이터를 덮어씀
	CDevice::GetInst()->GetContext()->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

	// buffer의 subresource에 데이터를 기록한다.
	memcpy(map.pData, data, mSize);

	// GPU Access를 이전 상태로 되돌린다.
	CDevice::GetInst()->GetContext()->Unmap(mBuffer, 0);

	// 각 쉐이더 파이프라인에 상수 버퍼를 할당한다.
	// register의 범위는 0 ~ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT(13)이다.
	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(mRegister, 1, &mBuffer);
	}

	if (meConstantBufferShaderType & (int)eConstantBufferShaderTypeFlags::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(mRegister, 1, &mBuffer);
	}
}
