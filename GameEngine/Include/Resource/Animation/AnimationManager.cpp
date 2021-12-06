#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"

CAnimationManager::CAnimationManager()	:
	mAnimation2DCBuffer(nullptr)
{
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(mAnimation2DCBuffer);
}

bool CAnimationManager::Init()
{
	mAnimation2DCBuffer = new CAnimation2DConstantBuffer;
	mAnimation2DCBuffer->Init();

	return true;
}

bool CAnimationManager::CreateAnimationSequence(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (sequence)
	{
		return true;
	}

	sequence = new CAnimationSequence2D;
	sequence->SetName(name);

	if (!sequence->Init(textureName, fileName, pathName))
	{
		assert(false);
		return false;
	}

	mMapSequence2D.insert(std::make_pair(name, sequence));

	return true;
}

void CAnimationManager::AddFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (!sequence)
	{
		return;
	}

	sequence->AddFrame(start, size);
}

void CAnimationManager::AddFrame(const std::string& name, const float startX, const float startY, const float width, const float height)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (!sequence)
	{
		return;
	}

	sequence->AddFrame(startX, startY, width, height);
}

CAnimationSequence2D* CAnimationManager::FindSequence(const std::string& name)
{
	auto iter = mMapSequence2D.find(name);

	if (iter == mMapSequence2D.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimationManager::ReleaseSequence(const std::string& name)
{
	auto iter = mMapSequence2D.find(name);
	
	if (iter != mMapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMapSequence2D.erase(iter);
		}
	}
}
