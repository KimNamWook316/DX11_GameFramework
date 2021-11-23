#include "GraphicShader.h"
#include "../../Device.h"
#include "../../PathManager.h"

CGraphicShader::CGraphicShader()
	: mInputLayout(nullptr)
	, mInputSize(0)
	, mVS(nullptr)
	, mPS(nullptr)
	, mHS(nullptr)
	, mDS(nullptr)
	, mGS(nullptr)
	, mVSBlob(nullptr)
	, mPSBlob(nullptr)
	, mHSBlob(nullptr)
	, mDSBlob(nullptr)
	, mGSBlob(nullptr)
{
	meType = eShaderType::GRAPHIC;
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(mInputLayout);

	SAFE_RELEASE(mVS);
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mHS);
	SAFE_RELEASE(mDS);
	SAFE_RELEASE(mGS);

	SAFE_RELEASE(mVSBlob);
	SAFE_RELEASE(mPSBlob);
	SAFE_RELEASE(mHSBlob);
	SAFE_RELEASE(mDSBlob);
	SAFE_RELEASE(mGSBlob);
}

bool CGraphicShader::Init()
{
	return false;
}

// ���� ������ �ִ� ���̴����� ���������ο� �����Ѵ�.
void CGraphicShader::SetShader()
{
	// �� �ܰ躰 ���̴����� �����Ѵ�.
	CDevice::GetInst()->GetContext()->VSSetShader(mVS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(mPS, nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(mHS, nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(mDS, nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(mGS, nullptr, 0);

	// Inputlayout�� �����Ѵ�.
	CDevice::GetInst()->GetContext()->IASetInputLayout(mInputLayout);
}

void CGraphicShader::AddInputDesc(const char* name, unsigned int index, DXGI_FORMAT fmt, 
	unsigned int slot, unsigned int size, 
	D3D11_INPUT_CLASSIFICATION slotClass, unsigned int instanceDataStep)
{
	D3D11_INPUT_ELEMENT_DESC desc = {};

	desc.SemanticName = name; // ���п� �ο��� ���ڿ� �̸�. ���̴����� semantic���� ���δ�.
	desc.SemanticIndex = index; // semantic�� �ο��� ����.
	desc.Format = fmt; // �� ������ �ڷ� ����
	desc.InputSlot = slot; // ���� ���� ������ ���� ( �� 16���� ���� ), ���⼭�� �ϳ��� �������� ���������ο� ������ ��
	desc.InputSlotClass = slotClass; // ��� ����� ����
	desc.InstanceDataStepRate = instanceDataStep; // ��� ��� (�ν��Ͻ�)�� ����

	// �Է� ������ �ϳ��� ����ϴ� ���, ���� ����ü�� ���� ��ġ�� �� ���� ������ ���� ��ġ ������ ������
	// ����, VertexColor�� 12byte ��ġ, 16byte �������� �̷���� �ִ�.
	// ��ġ ������ offset�� 0, ���� ������ offset�� 12�̴�.
	// �� �ڵ忡����, mInputSize�� ũ�⸦ �������� offset�� �ڵ����� �����ϰ� �ִ�.
	desc.AlignedByteOffset = mInputSize;

	mInputSize += size;
	mVecInputDesc.push_back(desc);
}

bool CGraphicShader::CreateInputLayout()
{
	// InputDesc �迭�� InputLayout�� �����Ѵ�.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(&mVecInputDesc[0], (UINT)mVecInputDesc.size(),
		mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), &mInputLayout)))
	{
		assert(false);
		return false;
	}

	return true;
}

bool CGraphicShader::LoadVertexShader(const char* entryName, const TCHAR* fileName, 
	const std::string& pathName)
{
	// ������ ���� ���̴� ������ �����Ѵ�.
	SAFE_RELEASE(mVS);
	SAFE_RELEASE(mVSBlob);

	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	// ���̴� ���� �������Ѵ�.
	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "vs_5_0", flag, 0, &mVSBlob, &error))) 
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	// ������ �� ���̴� ���Ϸκ��� ���̴� ��ü�� �����Ѵ�.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer(),
		mVSBlob->GetBufferSize(), nullptr, &mVS)))
	{
		assert(false);
		return false;
	}

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mPSBlob);

	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "ps_5_0", flag, 0, &mPSBlob, &error)))
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer(),
		mPSBlob->GetBufferSize(), nullptr, &mPS)))
	{
		assert(false);
		return false;
	}

	return true;
}

bool CGraphicShader::LoadHullShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(mHS);
	SAFE_RELEASE(mHSBlob);

	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "hs_5_0", flag, 0, &mHSBlob, &error)))
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateHullShader(mHSBlob->GetBufferPointer(),
		mHSBlob->GetBufferSize(), nullptr, &mHS)))
	{
		assert(false);
		return false;
	}

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(mDS);
	SAFE_RELEASE(mDSBlob);

	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "ds_5_0", flag, 0, &mDSBlob, &error)))
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDomainShader(mDSBlob->GetBufferPointer(),
		mDSBlob->GetBufferSize(), nullptr, &mDS)))
	{
		assert(false);
		return false;
	}

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(mGS);
	SAFE_RELEASE(mGSBlob);

	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "gs_5_0", flag, 0, &mDSBlob, &error)))
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateGeometryShader(mGSBlob->GetBufferPointer(),
		mGSBlob->GetBufferSize(), nullptr, &mGS)))
	{
		assert(false);
		return false;
	}

	return true;
}
