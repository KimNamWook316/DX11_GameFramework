#include "SceneResource.h"

CSceneResource::CSceneResource()	:
	mScene(nullptr)
{
}

CSceneResource::~CSceneResource()
{
	{
		auto iter = mMapMesh.begin();
		auto iterEnd = mMapMesh.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			// erase하면 다음 iterator 반환하며 반복
			// refCount--
			iter = mMapMesh.erase(iter);

			CResourceManager::GetInst()->ReleaseMesh(name);
		}
	}

	{
		auto iter = mMapShader.begin();
		auto iterEnd = mMapShader.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			iter = mMapShader.erase(iter);

			CResourceManager::GetInst()->ReleaseShader(name);
		}
	}

	{
		auto iter = mMapTexture.begin();
		auto iterEnd = mMapTexture.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			iter = mMapTexture.erase(iter);

			CResourceManager::GetInst()->ReleaseTexture(name);
		}
			
	}

	{
		auto iter = mMapMaterial.begin();
		auto iterEnd = mMapMaterial.end();

		for (; iter != iterEnd;)
		{
			std::string name = iter->first;

			iter = mMapMaterial.erase(iter);

			CResourceManager::GetInst()->ReleaseMaterial(name);
		}
	}
}

CMesh* CSceneResource::FindMesh(const std::string& name)
{
	auto iter = mMapMesh.find(name);

	if (iter == mMapMesh.end())
	{
		CMesh* findNotInScene = CResourceManager::GetInst()->FindMesh(name);

		if (!findNotInScene)
		{
			assert(false);
			return nullptr;
		}

		mMapMesh.insert(std::make_pair(name, findNotInScene));
		return findNotInScene;
	}

	return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& name)
{
	auto iter = mMapShader.find(name);

	if (iter == mMapShader.end())
	{
		CShader* findNotInScene = CResourceManager::GetInst()->FindShader(name);

		if (!findNotInScene)
		{
			assert(false);
			return nullptr;
		}

		mMapShader.insert(std::make_pair(name, findNotInScene));

		return findNotInScene;
	}

	return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& name)
{
	auto iter = mMapMaterial.find(name);

	if (iter == mMapMaterial.end())
	{
		CMaterial* findNotInScene = CResourceManager::GetInst()->FindMaterial(name);

		if (!findNotInScene)
		{
			assert(false);
			return nullptr;
		}

		mMapMaterial.insert(std::make_pair(name, findNotInScene));
		return findNotInScene;
	}

 	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (FindTexture(name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
	{
		assert(false);
		return false;
	}

	mMapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (FindTexture(name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
	{
		return false;
	}

	mMapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

CTexture* CSceneResource::FindTexture(const std::string& name)
{
	auto iter = mMapTexture.find(name);

	if (iter == mMapTexture.end())
	{
		CTexture* findNotInScene = CResourceManager::GetInst()->FindTexture(name);
		
		if (!findNotInScene)
		{
			return nullptr;
		}

		mMapTexture.insert(std::make_pair(name, findNotInScene));

		return findNotInScene;
	}

	return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, textureName, fileName, pathName))
	{
		assert(false);
		return false;
	}

	mMapSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequece2D(name)));
	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, texture))
	{
		assert(false);
		return false;
	}

	mMapSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequece2D(name)));
	return true;
}

void CSceneResource::AddAnimationSequece2DFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
	CAnimationSequence2D* anim = FindAnimationSequence2D(name);

	if (!anim)
	{
		return;
	}

	anim->AddFrame(start, size);
}

void CSceneResource::AddAnimationSequece2DFrame(const std::string& name, const float startX, const float startY, const float width, const float height)
{
	CAnimationSequence2D* anim = FindAnimationSequence2D(name);

	if (!anim)
	{
		return;
	}

	anim->AddFrame(startX, startY, width, height);
}

CAnimationSequence2D* CSceneResource::FindAnimationSequence2D(const std::string& name)
{
	auto iter = mMapSequence2D.find(name);

	if (iter == mMapSequence2D.end())
	{
		CAnimationSequence2D* findNotInScene = CResourceManager::GetInst()->FindAnimationSequece2D(name);

		if (!findNotInScene)
		{
			return nullptr;
		}

		mMapSequence2D.insert(std::make_pair(name, findNotInScene));

		return findNotInScene;
	}

	return iter->second;
}

CAnimation2DConstantBuffer* CSceneResource::GetAnimation2DCBuffer() const
{
	return CResourceManager::GetInst()->GetAnimation2DCBuffer();
}

void CSceneResource::ReleaseAnimationSequence2D(const std::string& name)
{
	auto iter = mMapSequence2D.find(name);

	if (iter != mMapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 2)
		{
			mMapSequence2D.erase(name);
			CResourceManager::GetInst()->ReleaseAnimationSequece2D(name);
		}
	}
}

bool CSceneResource::SaveSequence2DFullPath(const std::string& name, const char* fullPath)
{
	return CResourceManager::GetInst()->SaveSequence2DFullPath(name, fullPath);
}

bool CSceneResource::LoadSequence2DFullPath(const char* fullPath)
{
	std::string sequenceName;

	if (!CResourceManager::GetInst()->LoadSequence2DFullPath(sequenceName, fullPath, mScene))
	{
		return false;
	}
	
	if (FindAnimationSequence2D(sequenceName))
	{
		return true;
	}

	CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequece2D(sequenceName);

	mMapSequence2D.insert(std::make_pair(sequenceName, sequence));
	return true;
}

bool CSceneResource::LoadSequence2DFullPath(std::string& outName, const char* fullPath)
{
	if (!CResourceManager::GetInst()->LoadSequence2DFullPath(outName, fullPath, mScene))
	{
		return false;
	}

	if (FindAnimationSequence2D(outName))
	{
		return true;
	}

	CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequece2D(outName);
	
	mMapSequence2D.insert(std::make_pair(outName, sequence));
	return true;
}

bool CSceneResource::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return CResourceManager::GetInst()->SaveSequence2D(name, fileName, pathName);
}

bool CSceneResource::LoadSequence2D(const char* fileName, const std::string& pathName)
{
	std::string sequenceName;

	if (!CResourceManager::GetInst()->LoadSequence2D(sequenceName, fileName, pathName))
	{
		assert(false);
		return false;
	}
	
	if (FindAnimationSequence2D(sequenceName))
	{
		return true;
	}

	CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequece2D(sequenceName);
	mMapSequence2D.insert(std::make_pair(sequenceName, sequence));
	return true;
}

bool CSceneResource::LoadSequence2D(std::string& outName, const char* fileName, const std::string& pathName)
{
	if (!CResourceManager::GetInst()->LoadSequence2D(outName, fileName, pathName))
	{
		assert(false);
		return false;
	}

	if (FindAnimationSequence2D(outName))
	{
		return true;
	}

	CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequece2D(outName);
	mMapSequence2D.insert(std::make_pair(outName, sequence));
	return true;
}

bool CSceneResource::LoadSound(const std::string& channelGroupName, bool bLoop, const std::string& soundName, const char* fileName, const std::string& pathName)
{
	if (FindSound(soundName))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadSound(channelGroupName, bLoop, soundName, fileName, pathName))
	{
		return false;
	}

	mMapSound.insert(std::make_pair(soundName, CResourceManager::GetInst()->FindSound(soundName)));
	return true;
}

bool CSceneResource::CreateSoundChannelGroup(const std::string& groupName)
{
	return CResourceManager::GetInst()->CreateSoundChannelGroup(groupName);
}

bool CSceneResource::SetVolume(int volume)
{
	return CResourceManager::GetInst()->SetVolume(volume);
}

bool CSceneResource::SetVolume(const std::string& groupName, int volume)
{
	return CResourceManager::GetInst()->SetVolume(groupName, volume);
}

bool CSceneResource::SoundPlay(const std::string& soundName)
{
	return CResourceManager::GetInst()->SoundPlay(soundName);
}

bool CSceneResource::SoundStop(const std::string& soundName)
{
	return CResourceManager::GetInst()->SoundStop(soundName);
}

bool CSceneResource::SoundPause(const std::string& soundName)
{
	return CResourceManager::GetInst()->SoundPause(soundName);
}

bool CSceneResource::SoundResume(const std::string& soundName)
{
	return CResourceManager::GetInst()->SoundResume(soundName);
}

CSound* CSceneResource::FindSound(const std::string& soundName)
{
	auto iter = mMapSound.find(soundName);

	if (iter == mMapSound.end())
	{
		CSound* findNotInScene = CResourceManager::GetInst()->FindSound(soundName);
		
		if (!findNotInScene)
		{
			return nullptr;
		}
		
		return findNotInScene;
	}
	return iter->second;
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& groupName)
{
	return CResourceManager::GetInst()->FindChannelGroup(groupName);
}
