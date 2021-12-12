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

	char ext[_MAX_EXT] = {}; // Ȯ����
	char fullPathMultiByte[MAX_PATH] = {};

#ifdef UNICODE

	int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultiByte, convertLength, nullptr, nullptr);

#else

	strcpy_s(fullPathMultiByte, fullPath);

#endif // _DEBUG

	// ext�� Ȯ���ڸ� ��´�.
	_splitpath_s(fullPathMultiByte, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	// �빮�ڷ� ��ȯ
	_strupr_s(ext);

	ScratchImage* image = new ScratchImage;

	// Ȯ���ڿ� ���� ScratchImage ������ �ε�
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

	TCHAR _fileExt[_MAX_EXT] = {};

	_wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, info->FileName, MAX_PATH, _fileExt, _MAX_EXT);

	lstrcat(info->FileName, _fileExt);

	info->PathName = new char[MAX_PATH];
	memset(info->PathName, 0, sizeof(char) * MAX_PATH);

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

	// ScratchImage�� ��� �ȼ� ������ ���̴��� ������ ����ϱ� ���� ���ҽ� �� ����
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
	// ���̴����ҽ� ���̴��� �ѱ��
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
