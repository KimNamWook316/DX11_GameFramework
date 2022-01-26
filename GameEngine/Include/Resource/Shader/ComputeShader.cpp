#include "ComputeShader.h"
#include "../../PathManager.h"
#include "../../Device.h";

CComputeShader::CComputeShader()	:
	mCS(nullptr),
	mCSBlob(nullptr)
{
	meType = eShaderType::COMPUTE;
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(mCS);
	SAFE_RELEASE(mCSBlob);
}

void CComputeShader::SetShader()
{
	CDevice::GetInst()->GetContext()->CSSetShader(mCS, nullptr, 0);
}

bool CComputeShader::LoadComputeShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(mCS);
	SAFE_RELEASE(mCSBlob);

	unsigned int flag = 0;

#ifdef _DEBUG
	flag |= D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "cs_5_0", flag, 0, &mCSBlob, &error)))
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA(("\n"));
		assert(false);
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer(),
		mCSBlob->GetBufferSize(), nullptr, &mCS)))
	{
		assert(false);
		return false;
	}

	return true;
}

// 각 x, y, z 그룹 수
void CComputeShader::Excute(unsigned int x, unsigned int y, unsigned int z)
{
	// ComputShader를 실행시킨다.
	SetShader();
	CDevice::GetInst()->GetContext()->Dispatch(x, y, z);
}
