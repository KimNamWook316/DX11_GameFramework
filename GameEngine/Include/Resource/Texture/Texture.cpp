#include "Texture.h"
#include "../../PathManager.h"

CTexture::CTexture()
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
	lstrcpy(fullPath, fileName);

	info->FullPath = fullPath;

	mVecTextureInfo.push_back(info);
	return true;
}
