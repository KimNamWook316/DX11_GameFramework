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
	// � ���̴��� ���޵� �뵵���� �Ǵ��ϴ� Enum ����
	meConstantBufferShaderType = eConstantBufferType;

	D3D11_BUFFER_DESC desc = {};
	
	desc.Usage = D3D11_USAGE_DYNAMIC;	// GPU�� �а�, CPU�� ����.
	desc.ByteWidth = size;				// ������ ũ��
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���۷� ���ε�
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU���� ���� ������� �ϹǷ�

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

	// mBuffer ���긮�ҽ��� ���� ������(map ������)�� ������, GPU Access�� �����Ѵ�.
	// CPU�� �������� ���� �����ϰڴ�. ��, buffer�� usage�� DYNAMIC�� �� ����
	// D3D11_MAP_WRITE_DISCARD : ���� �����͸� ���
	CDevice::GetInst()->GetContext()->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

	// buffer�� subresource�� �����͸� ����Ѵ�.
	memcpy(map.pData, data, mSize);

	// GPU Access�� ���� ���·� �ǵ�����.
	CDevice::GetInst()->GetContext()->Unmap(mBuffer, 0);

	// �� ���̴� ���������ο� ��� ���۸� �Ҵ��Ѵ�.
	// register�� ������ 0 ~ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT(13)�̴�.
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
