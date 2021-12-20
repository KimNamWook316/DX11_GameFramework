#include "AnimationSequence2D.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"

CAnimationSequence2D::CAnimationSequence2D()	:
	mScene(nullptr)
{
}

CAnimationSequence2D::~CAnimationSequence2D()
{
}

bool CAnimationSequence2D::Init(CTexture* texture)
{
	mTexture = texture;
	return true;
}

bool CAnimationSequence2D::Init(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	// ¾ÀÀÌ ÀÖÀ¸¸é ¾À¿¡¼­ ·Îµå
	if (mScene)
	{
		if (!mScene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return false;
		}
		
		mTexture = mScene->GetResource()->FindTexture(name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return false;
		}

		mTexture = CResourceManager::GetInst()->FindTexture(name);
	}

	return true;
}

void CAnimationSequence2D::AddFrame(const Vector2& startPos, const Vector2& size)
{
	AnimationFrameData data = {};

	data.Start = startPos;
	data.Size = size;

	mVecFrameData.push_back(data);
}

void CAnimationSequence2D::AddFrame(const float startX, const float startY, const float width, const float height)
{
	AnimationFrameData data = {};

	data.Start = Vector2(startX, startY);
	data.Size = Vector2(width, height);

	mVecFrameData.push_back(data);
}

void CAnimationSequence2D::DeleteFrame(const int idx)
{
	auto iter = mVecFrameData.begin() + idx;
	mVecFrameData.erase(iter);
}

void CAnimationSequence2D::ClearFrame()
{
	mVecFrameData.clear();
}

void CAnimationSequence2D::Save(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "wt");

	if (!pFile)
	{
		assert(false);
		return;
	}

	Save(pFile);

	fclose(pFile);
}

void CAnimationSequence2D::Save(FILE* fp)
{
	int length = (int)mName.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mName.c_str(), sizeof(char), length, fp);

	bool bTextEnable = false;

	if (mTexture)
	{
		bTextEnable = true;
	}
	
	fwrite(&bTextEnable, sizeof(bool), 1, fp);

	if (mTexture)
	{
		mTexture->Save(fp);
	}

	int frameCount = (int)mVecFrameData.size();

	fwrite(&frameCount, sizeof(int), 1, fp);

	fwrite(&mVecFrameData[0], sizeof(AnimationFrameData), frameCount, fp);
}

void CAnimationSequence2D::Load(const char* fullPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, fullPath, "rt");

	if (!pFile)
	{
		assert(false);
		return;
	}

	Load(pFile);

	fclose(pFile);
}

void CAnimationSequence2D::Load(FILE* fp)
{
	int length = 0;
	fread(&length, sizeof(int), 1, fp);

	char name[256] = {};
	fread(name, sizeof(char), length, fp);
	mName = name;

	bool bTexEnable = false;
	fread(&bTexEnable, sizeof(bool), 1, fp);

	if (bTexEnable)
	{
		int texNameLength = 0;
		fread(&texNameLength, sizeof(int), 1, fp);
		char texName[256] = {};
		fread(texName, sizeof(char), texNameLength, fp);

		eImageType eType;
		fread(&eType, sizeof(eImageType), 1, fp);
		
		int infoCount = 0;
		fread(&infoCount, sizeof(int), 1, fp);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string pathName;

		for (int i = 0; i < infoCount; ++i)
		{
			int pathSize = 0;
			fread(&pathSize, sizeof(int), 1, fp);

			TCHAR fullPath[MAX_PATH] = {};
			fread(fullPath, sizeof(TCHAR), pathSize, fp);
			vecFullPath.push_back(fullPath);

			fread(&pathSize, sizeof(int), 1, fp);

			TCHAR texFileName[MAX_PATH] = {};
			fread(texFileName, sizeof(TCHAR), pathSize, fp);
			vecFileName.push_back(texFileName);
			
			fread(&pathSize, sizeof(int), 1, fp);

			char texPathName[MAX_PATH] = {};
			fread(texPathName, sizeof(char), pathSize, fp);

			pathName = texPathName;
		}

		switch (eType)
		{
		case eImageType::Atlas:
			if (vecFileName.size() == 1)
			{
				if (mScene)
				{
					mScene->GetResource()->LoadTexture(texName, vecFileName[0].c_str(), pathName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(texName, vecFileName[0].c_str(), pathName);
				}
			}
			else
			{

			}
			break;
		default:
			assert(false);
			break;
		}

		if (mScene)
		{
			mTexture = mScene->GetResource()->FindTexture(texName);
		}
		else
		{
			mTexture = CResourceManager::GetInst()->FindTexture(texName);
		}
	}

	int frameCount = 0;
	fread(&frameCount, sizeof(int), 1, fp);
	
	mVecFrameData.resize((const size_t)frameCount);

	fread(&mVecFrameData[0], sizeof(AnimationFrameData), frameCount, fp);
}

void CAnimationSequence2D::SetScene(CScene* scene)
{
	mScene = scene;
}
