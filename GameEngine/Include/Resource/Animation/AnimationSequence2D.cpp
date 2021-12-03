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

void CAnimationSequence2D::SetScene(CScene* scene)
{
	mScene = scene;
}
