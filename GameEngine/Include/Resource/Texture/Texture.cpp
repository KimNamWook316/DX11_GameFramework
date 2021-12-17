#include "Texture.h"
#include "../../Device.h"
#include "../../PathManager.h"

CTexture::CTexture()	:
	mScene(nullptr),
	meImageType(eImageType::Atlas)
{
}

CTexture::~CTexture()
{
	size_t size = mVecTextureInfo.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mVecTextureInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	TextureResourceInfo* info = new TextureResourceInfo;

	SetName(name);
	
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	TCHAR* fullPath = new TCHAR[MAX_PATH];
	memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);

	if (path)
	{
		lstrcpy(fullPath, path->Path);
	}
	lstrcat(fullPath, fileName);

	info->FullPath = fullPath;

	info->FileName = new TCHAR[MAX_PATH];
	memset(info->FileName, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(info->FileName, fileName);

	info->PathName = new char[MAX_PATH];
	memset(info->PathName, 0, sizeof(char) * MAX_PATH);

	strcpy_s(info->PathName, pathName.length() + 1, pathName.c_str());

	char ext[_MAX_EXT] = {}; // 확장자
	char fullPathMultiByte[MAX_PATH] = {};

#ifdef UNICODE

	int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultiByte, convertLength, nullptr, nullptr);

#else

	strcpy_s(fullPathMultiByte, fullPath);

#endif // _DEBUG

	// ext에 확장자만 담는다.
	_splitpath_s(fullPathMultiByte, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	// 대문자로 변환
	_strupr_s(ext);

	ScratchImage* image = new ScratchImage;

	// 확장자에 따라 ScratchImage 변수에 로드
	if (strcmp(ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			assert(false);
			return false;
		}
	}

	if (strcmp(ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(fullPath, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			assert(false);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			assert(false);
			return false;
		}
	}

	info->Image = image;

	mVecTextureInfo.push_back(info);

	return createResource(0);
}

bool CTexture::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	TextureResourceInfo* info = new TextureResourceInfo;

	SetName(name);

	TCHAR* fullPath1 = new TCHAR[MAX_PATH];
	memset(fullPath1, 0, sizeof(TCHAR) * MAX_PATH);

	lstrcpy(fullPath1, fullPath);

	info->FullPath = fullPath1;

	info->FileName = new TCHAR[MAX_PATH];
	memset(info->FileName, 0, sizeof(TCHAR) * MAX_PATH);

	int pathLength = lstrlen(info->FullPath);

	// D:\Lecture\37th\API\GameFramework\GameFramework\Bin\Texture\Test.png
	// 위와 같은 경로가 있을 때, Texture\Text.png를 파일명으로 취급
	for (int i = pathLength - 1; i > 0; --i)
	{
		if (info->FullPath[i] == '\\' && i >= 4)
		{
			if ((info->FullPath[i - 1] == 'n' || info->FullPath[i - 1] == 'N') &&
				(info->FullPath[i - 2] == 'i' || info->FullPath[i - 2] == 'I') &&
				(info->FullPath[i - 3] == 'b' || info->FullPath[i - 3] == 'B') &&
				(info->FullPath[i - 4] == '\\'))
			{
				lstrcpy(info->FileName, &info->FullPath[i + 1]);
			}
		}
	}

	TCHAR _fileExt[_MAX_EXT] = {};

	_wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, _fileExt, _MAX_EXT);

	info->PathName = new char[MAX_PATH];
	memset(info->PathName, 0, sizeof(char) * MAX_PATH);

	strcpy_s(info->PathName, strlen(ROOT_PATH) + 1, ROOT_PATH);

	char ext[_MAX_EXT];

#ifdef UNICODE
	int convertLength = WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, ext, convertLength, nullptr, nullptr);
#else
	strcpy_s(ext, _fileExt);
#endif
	
	_strupr_s(ext);

	ScratchImage* image = new ScratchImage;

	if (strcmp(ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(fullPath, DDS_FLAGS_NONE, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			return false;
		}
	}

	else if (strcmp(ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(fullPath, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(fullPath, WIC_FLAGS_NONE, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			return false;
		}
	}

	info->Image = image;
	mVecTextureInfo.push_back(info);

	return createResource(0);
}

bool CTexture::createResource(const int index)
{
	TextureResourceInfo* info = mVecTextureInfo[index];

	// ScratchImage에 담긴 픽셀 정보를 쉐이더로 연결해 출력하기 위한 리소스 뷰 생성
	if (FAILED(CreateShaderResourceView(CDevice::GetInst()->GetDevice(), info->Image->GetImages(),
		info->Image->GetImageCount(), info->Image->GetMetadata(), &info->ShaderResourceView)))
	{
		assert(false);
		return false;
	}

	info->Width = (unsigned int)info->Image->GetImages()[0].width;
	info->Height = (unsigned int)info->Image->GetImages()[0].height;

	return true;
}

void CTexture::SetShader(const int reg, const int eShaderType, const int index)
{
	// 쉐이더리소스 쉐이더에 넘기기
	if (meImageType != eImageType::Array)
	{
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &mVecTextureInfo[index]->ShaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &mVecTextureInfo[index]->ShaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &mVecTextureInfo[index]->ShaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &mVecTextureInfo[index]->ShaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &mVecTextureInfo[index]->ShaderResourceView);
		}
	}
	else
	{

	}
}

void CTexture::ResetShader(const int reg, const int eShaderType, const int index)
{
	ID3D11ShaderResourceView* shaderResourceView = nullptr;

	if (meImageType != eImageType::Array)
	{
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &shaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &shaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &shaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &shaderResourceView);
		}
		if (eShaderType & (int)eConstantBufferShaderTypeFlags::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &shaderResourceView);
		}
	}
	else
	{

	}
}

void CTexture::Save(FILE* pFile)
{
	int length = (int)mName.length();
	fwrite(&length, sizeof(int),1,  pFile);
	fwrite(mName.c_str(), sizeof(char), length, pFile);

	fwrite(&meImageType, sizeof(eImageType), 1, pFile);

	int infoCount = (int)mVecTextureInfo.size();
	fwrite(&infoCount, sizeof(int), 1, pFile);

	for (int i = 0; i < infoCount; ++i)
	{
		int pathSize = (int)lstrlen(mVecTextureInfo[i]->FullPath);

		fwrite(&pathSize, sizeof(int), 1, pFile);
		fwrite(mVecTextureInfo[i]->FullPath, sizeof(TCHAR), pathSize, pFile);

		pathSize = (int)lstrlen(mVecTextureInfo[i]->FileName);

		fwrite(&pathSize, sizeof(int), 1, pFile);
		fwrite(mVecTextureInfo[i]->FileName, sizeof(TCHAR), pathSize, pFile);

		pathSize = (int)strlen(mVecTextureInfo[i]->PathName);
		
		fwrite(&pathSize, sizeof(int), 1, pFile);
		fwrite(mVecTextureInfo[i]->PathName, sizeof(char), pathSize, pFile);
	}
}
