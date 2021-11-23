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

// 현재 가지고 있는 쉐이더들을 파이프라인에 설정한다.
void CGraphicShader::SetShader()
{
	// 각 단계별 쉐이더들을 설정한다.
	CDevice::GetInst()->GetContext()->VSSetShader(mVS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(mPS, nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(mHS, nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(mDS, nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(mGS, nullptr, 0);

	// Inputlayout을 설정한다.
	CDevice::GetInst()->GetContext()->IASetInputLayout(mInputLayout);
}

void CGraphicShader::AddInputDesc(const char* name, unsigned int index, DXGI_FORMAT fmt, 
	unsigned int slot, unsigned int size, 
	D3D11_INPUT_CLASSIFICATION slotClass, unsigned int instanceDataStep)
{
	D3D11_INPUT_ELEMENT_DESC desc = {};

	desc.SemanticName = name; // 성분에 부여된 문자열 이름. 쉐이더에서 semantic으로 쓰인다.
	desc.SemanticIndex = index; // semantic에 부여된 색인.
	desc.Format = fmt; // 이 성분의 자료 형식
	desc.InputSlot = slot; // 정점 버퍼 슬롯의 색인 ( 총 16개가 있음 ), 여기서는 하나의 슬롯으로 파이프라인에 공급할 것
	desc.InputSlotClass = slotClass; // 고급 기법에 쓰임
	desc.InstanceDataStepRate = instanceDataStep; // 고급 기법 (인스턴싱)에 쓰임

	// 입력 슬롯을 하나만 사용하는 경우, 정점 구조체의 시작 위치와 이 정점 성분의 시작 위치 사이의 오프셋
	// 예로, VertexColor는 12byte 위치, 16byte 색상으로 이루어져 있다.
	// 위치 정보의 offset은 0, 색상 정보의 offset은 12이다.
	// 이 코드에서는, mInputSize에 크기를 누적시켜 offset을 자동으로 적용하고 있다.
	desc.AlignedByteOffset = mInputSize;

	mInputSize += size;
	mVecInputDesc.push_back(desc);
}

bool CGraphicShader::CreateInputLayout()
{
	// InputDesc 배열로 InputLayout을 생성한다.
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
	// 이전에 쓰던 쉐이더 정보를 제거한다.
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

	// 쉐이더 파일 컴파일한다.
	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "vs_5_0", flag, 0, &mVSBlob, &error))) 
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	// 컴파일 된 쉐이더 파일로부터 쉐이더 객체를 생성한다.
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
