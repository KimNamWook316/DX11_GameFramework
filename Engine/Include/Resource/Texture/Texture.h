#pragma once

#include "../../Ref.h"

struct TextureResourceInfo
{
	// Texture의 pixel 정보를 담아놓기 위한 자료형
	ScratchImage* Image;

	// 픽셀 정보를 이용해서 쉐이더 리소스 뷰를 만들어놓고 사용함
	// 쉐이더에다가 넘겨줘서 가져다가 쓸 수 있게끔 만들어주는 리소스
	// 텍스쳐를 로딩해놓고 메쉬에다가 출력하기 위해서 쉐이더로 넘겨줘서 픽셀쉐이더에서
	// 색깔을 뽑아다가 맵핑하는 방식으로 동작한다.
	ID3D11ShaderResourceView* ShaderResourceView;
	
	unsigned int Width;
	unsigned int Height;
	
	// 세이브 / 로드를 위한 변수
	TCHAR* FileName;
	char* PathName;
	TCHAR* FullPath;

	TextureResourceInfo()	:
		Image(nullptr),
		ShaderResourceView(nullptr),
		FileName(nullptr),
		PathName(nullptr),
		FullPath(nullptr),
		Width(0),
		Height(0)
	{
	}

	~TextureResourceInfo()
	{
		SAFE_RELEASE(ShaderResourceView);
		SAFE_DELETE_ARRAY(FileName);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_DELETE_ARRAY(FullPath);
		SAFE_DELETE(Image);
	}
};

class CTexture : public CRef
{
	friend class CTextureManager;

protected:
	CTexture();
	virtual ~CTexture();

public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);

public:
	void SetShader(const int reg, const int eShaderType, const int index);
	void ResetShader(const int reg, const int eShaderType, const int index);

public:
	void Save(FILE* pFile);

public:
	eImageType GetImageType() const
	{
		return meImageType;
	}

	unsigned int GetWidth(const int index = 0) const
	{
		return mVecTextureInfo[index]->Width;
	}

	unsigned int GetHeight(const int index = 0) const
	{
		return mVecTextureInfo[index]->Height;
	}

	size_t GetImageCount() const
	{
		return mVecTextureInfo.size();
	}

	ID3D11ShaderResourceView* GetResourceView(const int idx = 0)
	{
		return mVecTextureInfo[idx]->ShaderResourceView;
	}

private:
	bool createResource(const int index);

protected:
	class CScene* mScene;
	std::vector<TextureResourceInfo*> mVecTextureInfo;
	eImageType meImageType;
};

