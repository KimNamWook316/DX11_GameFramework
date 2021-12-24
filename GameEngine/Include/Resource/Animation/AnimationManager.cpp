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

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
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

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (sequence)
	{
		return true;
	}

	sequence = new CAnimationSequence2D;

	sequence->SetName(name);

	if (!sequence->Init(texture))
	{
		assert(false);
		SAFE_DELETE(sequence);
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

bool CAnimationManager::SaveSequenceFullPath(const std::string& name, const char* fullPath)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (!sequence)
	{
		return false;
	}

	sequence->SaveFullPath(fullPath);
	return true;
}

bool CAnimationManager::LoadSequenceFullPath(std::string& outName, const char* fullPath, CScene* scene)
{
	CAnimationSequence2D* sequence = new CAnimationSequence2D;
	
	sequence->SetScene(scene);

	if(!sequence->LoadFullPath(fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	outName = sequence->GetName();

	if (FindSequence(outName))
	{
		SAFE_RELEASE(sequence);
		return true;
	}
	mMapSequence2D.insert(std::make_pair(sequence->GetName(), sequence));
	return true;
}

bool CAnimationManager::SaveSequence(const std::string& name, const char* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (!sequence)
	{
		return false;
	}

	sequence->Save(fileName, pathName);
	return true;
}

bool CAnimationManager::LoadSequece(std::string& outName, const char* fileName, const std::string& pathName, CScene* scene)
{
	CAnimationSequence2D* sequence = new CAnimationSequence2D;
	sequence->SetScene(scene);

	if (!sequence->Load(fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	outName = sequence->GetName();

	if (FindSequence(outName))
	{
		SAFE_RELEASE(sequence);
		return true;
	}

	mMapSequence2D.insert(std::make_pair(outName, sequence));
	return true;
}
