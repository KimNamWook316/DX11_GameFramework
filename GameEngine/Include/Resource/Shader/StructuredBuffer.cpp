#include "StructuredBuffer.h"
#include "../../Device.h"

CStructuredBuffer::CStructuredBuffer()	:
	mBuffer(nullptr),
	mSRV(nullptr),
	mUAV(nullptr),
	mDesc{}
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& buffer)
{
	*this = buffer;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&mDesc, nullptr, &mBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = mCount;

	CDevice::GetInst()->GetDevice()->CreateShaderResourceView(mBuffer, &srvDesc, &mSRV);

	if (!mbDynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = 0;

		CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(mBuffer, &uavDesc, &mUAV);
	}

}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(mSRV);
	SAFE_RELEASE(mUAV);
	SAFE_RELEASE(mBuffer);
}

bool CStructuredBuffer::Init(const std::string& name, const unsigned int size, const unsigned int count,
	const int reg, const bool bDynamic, const int structuredBufferShaderTypeFlag)
{
	SAFE_RELEASE(mSRV);
	SAFE_RELEASE(mUAV);
	SAFE_RELEASE(mBuffer);

	mName = name;
	mSize = size;
	mCount = count;
	mRegister = reg;
	mbDynamic = bDynamic;
	mStructuredBufferShaderType = structuredBufferShaderTypeFlag;

	mDesc.ByteWidth = mSize * mCount;
	mDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	mDesc.StructureByteStride = mSize;

	// Read / Write Buffer
	if (mbDynamic)
	{
		mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		mDesc.Usage = D3D11_USAGE_DYNAMIC;
		mDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}
	else
	{
		mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&mDesc, nullptr, &mBuffer)))
	{
		assert(false);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = mCount;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(mBuffer, &srvDesc, &mSRV)))
	{
		assert(false);
		return false;
	}

	// Compute Shader의 출력을 위해서 UAV가 필요함
	if (!mbDynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = 0;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(mBuffer, &uavDesc, &mUAV)))
		{
			assert(false);
			return false;
		}
	}

	return true;
}

void CStructuredBuffer::UpdateBuffer(void* data, int count)
{
	if (!mbDynamic)
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE map = {};

	CDevice::GetInst()->GetContext()->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

	memcpy(map.pData, data, (size_t)mSize * (size_t)mCount);

	CDevice::GetInst()->GetContext()->Unmap(mBuffer, 0);
}

void CStructuredBuffer::SetShader()
{
	if (mbDynamic)
	{
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(mRegister, 1, &mSRV);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(mRegister, 1, &mSRV);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(mRegister, 1, &mSRV);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(mRegister, 1, &mSRV);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(mRegister, 1, &mSRV);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(mRegister, 1, &mSRV);
		}
	}
	else
	{
		unsigned int count = 1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(mRegister, 1, &mUAV, &count);
	}
}

void CStructuredBuffer::SetShader(const int reg, const int structuredBufferShaderTypeFlag)
{
	if (mbDynamic)
	{
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &mSRV);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &mSRV);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &mSRV);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &mSRV);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &mSRV);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &mSRV);
		}
	}
	else
	{
		unsigned int count = 1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(reg, 1, &mUAV, &count);
	}
}

void CStructuredBuffer::ResetShader()
{
	if (mbDynamic)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(mRegister, 1, &srv);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(mRegister, 1, &srv);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(mRegister, 1, &srv);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(mRegister, 1, &srv);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(mRegister, 1, &srv);
		}
		if (mStructuredBufferShaderType & (int)eBufferShaderTypeFlags::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(mRegister, 1, &srv);
		}
	}
	else
	{
		unsigned int count = 1;
		ID3D11UnorderedAccessView* uav = nullptr;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(mRegister, 1, &uav, &count);
	}
}

void CStructuredBuffer::ResetShader(const int reg, const int structuredBufferShaderTypeFlag)
{
	if (mbDynamic)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &srv);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &srv);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &srv);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &srv);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &srv);
		}
		if (structuredBufferShaderTypeFlag & (int)eBufferShaderTypeFlags::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &srv);
		}
	}
	else
	{
		unsigned int count = 1;
		ID3D11UnorderedAccessView* uav = nullptr;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(reg, 1, &uav, &count);
	}
}

void CStructuredBuffer::CopyData(void* data)
{
	D3D11_MAPPED_SUBRESOURCE map = {};

	CDevice::GetInst()->GetContext()->Map(mBuffer, 0, D3D11_MAP_READ, 0, &map);
	memcpy(data, map.pData, (size_t)mSize * (size_t)mCount);
	CDevice::GetInst()->GetContext()->Unmap(mBuffer, 0);
}

void CStructuredBuffer::CopyResource(CStructuredBuffer* buffer)
{
	CDevice::GetInst()->GetContext()->CopyResource(buffer->mBuffer, mBuffer);
}

CStructuredBuffer* CStructuredBuffer::Clone()
{
	return new CStructuredBuffer(*this);
}
