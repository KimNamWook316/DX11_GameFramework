#pragma once

#include "DirectXTex.h"
#include "../../Ref.h"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif // _DEBUG

struct TextureResourceInfo
{
	// Texture�� pixel ������ ��Ƴ��� ���� �ڷ���
	ScratchImage* Image;

	// �ȼ� ������ �̿��ؼ� ���̴� ���ҽ� �並 �������� �����
	// ���̴����ٰ� �Ѱ��༭ �����ٰ� �� �� �ְԲ� ������ִ� ���ҽ�
	// �ؽ��ĸ� �ε��س��� �޽����ٰ� ����ϱ� ���ؼ� ���̴��� �Ѱ��༭ �ȼ����̴�����
	// ������ �̾ƴٰ� �����ϴ� ������� �����Ѵ�.
	ID3D11ShaderResourceView* ShaderResourceView;
	
	unsigned int Width;
	unsigned int Height;
	
	// ���̺� / �ε带 ���� ����
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

public:
	void SetShader(const int reg, const int eShaderType, const int index);
	void ResetShader(const int reg, const int eShaderType, const int index);

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

private:
	bool createResource(const int index);

protected:
	class CScene* mScene;
	std::vector<TextureResourceInfo*> mVecTextureInfo;
	eImageType meImageType;
};
