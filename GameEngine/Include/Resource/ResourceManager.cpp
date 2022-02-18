#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
	mMeshManager(nullptr),
	mShaderManager(nullptr),
	mMaterialManager(nullptr),
	mTextureManager(nullptr),
	mAnimationManager(nullptr),
	mSoundManager(nullptr),
	mFontManager(nullptr),
	mParticleManager(nullptr),
	mExcelManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(mAnimationManager);
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mShaderManager);
	SAFE_DELETE(mMaterialManager);
	SAFE_DELETE(mTextureManager);
	SAFE_DELETE(mSoundManager);
	SAFE_DELETE(mFontManager);
	SAFE_DELETE(mParticleManager);
	SAFE_DELETE(mExcelManager);
}

bool CResourceManager::Init()
{
	mMeshManager = new CMeshManager;
	if (!mMeshManager->Init())
	{
		assert(false);
		return false;
	}

	mShaderManager = new CShaderManager;
	if (!mShaderManager->Init())
	{
		assert(false);
		return false;
	}

	mTextureManager = new CTextureManager;
	if (!mTextureManager->Init())
	{
		assert(false);
		return false;
	}

	mMaterialManager = new CMaterialManager;
	if (!mMaterialManager->Init())
	{
		assert(false);
		return false;
	}

	mAnimationManager = new CAnimationManager;
	if (!mAnimationManager->Init())
	{
		assert(false);
		return false;
	}

	mSoundManager = new CSoundManager;
	if (!mSoundManager->Init())
	{
		assert(false);
		return false;
	}

	mFontManager = new CFontManager;
	if (!mFontManager->Init())
	{
		assert(false);
		return false;
	}

	mParticleManager = new CParticleManager;
	if (!mParticleManager->Init())
	{
		assert(false);
		return false;
	}

	mExcelManager = new CExcelManager;
	if (!mExcelManager->Init())
	{
		assert(false);
		return false;
	}

	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& name)
{	
	return mMeshManager->FindMesh(name);
}

void CResourceManager::ReleaseMesh(const std::string& name)
{
	return mMeshManager->ReleaseMesh(name);
}

CShader* CResourceManager::FindShader(const std::string& name)
{
	return mShaderManager->FindShader(name);
}

void CResourceManager::ReleaseShader(const std::string& name)
{
	return mShaderManager->ReleaseShader(name);
}

bool CResourceManager::CreateConstantBuffer(const std::string& name, const int size, const int reg, const int eConstantBufferShaderType)
{
	return mShaderManager->CreateConstantBuffer(name, size, reg, eConstantBufferShaderType);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& name)
{
	return mShaderManager->FindConstantBuffer(name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& name)
{
	return mMaterialManager->FindMaterial(name);
}

void CResourceManager::ReleaseMaterial(const std::string& name)
{
	return mMaterialManager->ReleaseMaterial(name);
}

CTexture* CResourceManager::FindTexture(const std::string& name)
{
	return mTextureManager->FindTexture(name);
}

void CResourceManager::ReleaseTexture(const std::string& name)
{
	mTextureManager->ReleaseTexture(name);
}

bool CResourceManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& filePath)
{
	return mTextureManager->LoadTexture(name, fileName, filePath);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	return mTextureManager->LoadTextureFullPath(name, fullPath);
}

bool CResourceManager::LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& path)
{
	return mTextureManager->LoadTexture(name, vecFileName, path);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	return mTextureManager->LoadTexture(name, vecFullPath);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	return mAnimationManager->CreateAnimationSequence2D(name, textureName, fileName, pathName);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	return mAnimationManager->CreateAnimationSequence2D(name, texture);
}

void CResourceManager::AddAnimationSequece2DFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
	mAnimationManager->AddFrame(name, start, size);
}

void CResourceManager::AddAnimationSequece2DFrame(const std::string& name, const float startX, const float startY, const float width, const float height)
{
	mAnimationManager->AddFrame(name, startX, startY, width, height);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequece2D(const std::string& name)
{
	return mAnimationManager->FindSequence(name);
}

void CResourceManager::ReleaseAnimationSequece2D(const std::string& name)
{
	mAnimationManager->ReleaseSequence(name);
}

CAnimation2DConstantBuffer* CResourceManager::GetAnimation2DCBuffer() const
{
	return mAnimationManager->GetAnimation2DCBuffer();
}

bool CResourceManager::SaveSequence2DFullPath(const std::string& name, const char* fullPath)
{
	return mAnimationManager->SaveSequenceFullPath(name, fullPath);
}

bool CResourceManager::LoadSequence2DFullPath(std::string& outName, const char* fullPath, CScene* scene)
{
	return mAnimationManager->LoadSequenceFullPath(outName, fullPath, scene);
}

bool CResourceManager::LoadSequence2DFullPath(const char* fullPath, CScene* scene)
{
	std::string name;
	return mAnimationManager->LoadSequenceFullPath(name, fullPath, scene);
}

bool CResourceManager::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return mAnimationManager->SaveSequence(name, fileName, pathName);
}

bool CResourceManager::LoadSequence2D(const char* fileName, const std::string& pathName, CScene* scene)
{
	std::string name;
	return mAnimationManager->LoadSequece(name, fileName, pathName, scene);
}

bool CResourceManager::LoadSequence2D(std::string& outName, const char* fileName, const std::string& pathName, CScene* scene)
{
	return mAnimationManager->LoadSequece(outName, fileName, pathName, scene);
}

bool CResourceManager::LoadSound(const std::string& channelGroupName, bool bLoop, const std::string& soundName, const char* fileName, const std::string& pathName)
{
	return mSoundManager->LoadSound(channelGroupName, bLoop, soundName, fileName, pathName);
}

bool CResourceManager::CreateSoundChannelGroup(const std::string& groupName)
{
	return mSoundManager->CreateSoundChannelGroup(groupName);
}

bool CResourceManager::SetVolume(int volume)
{
	return mSoundManager->SetVolume(volume);
}

bool CResourceManager::SetVolume(const std::string& groupName, int volume)
{
	return mSoundManager->SetVolume(groupName, volume);
}

bool CResourceManager::SoundPlay(const std::string& soundName)
{
	return mSoundManager->SoundPlay(soundName);
}

bool CResourceManager::SoundStop(const std::string& soundName)
{
	return mSoundManager->SoundStop(soundName);
}

bool CResourceManager::SoundPause(const std::string& soundName)
{
	return mSoundManager->SoundPause(soundName);
}

bool CResourceManager::SoundResume(const std::string& soundName)
{
	return mSoundManager->SoundResume(soundName);
}

CSound* CResourceManager::FindSound(const std::string& soundName)
{
	return mSoundManager->FindSound(soundName);
}

FMOD::ChannelGroup* CResourceManager::FindChannelGroup(const std::string& groupName)
{
	return mSoundManager->FindChannelGroup(groupName);
}

void CResourceManager::ReleaseSound(const std::string& soundName)
{
	mSoundManager->ReleaseSound(soundName);
}

bool CResourceManager::CreateFontFile(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return mFontManager->CreateFontFile(name, fileName, pathName);
}

bool CResourceManager::LoadFont(const std::string& name, const TCHAR* fontName, const int weight, const float fontSize, const TCHAR* localName, const int stretch)
{
	return mFontManager->LoadFont(name, fontName, weight, fontSize, localName, stretch);
}

const TCHAR* CResourceManager::GetFontFaceName(const std::string& name)
{
	return mFontManager->GetFontFaceName(name);
}

const char* CResourceManager::GetFontFaceNameMultybyte(const std::string& name)
{
	return mFontManager->GetFontFaceNameMultybyte(name);
}

bool CResourceManager::CreateFontColor(const float r, const float g, const float b, const float a)
{
	return mFontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	return mFontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(const Vector4& color)
{
	return mFontManager->CreateFontColor(color);
}

bool CResourceManager::CreateFontColor(const unsigned int color)
{
	return mFontManager->CreateFontColor(color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const float r, const float g, const float b, const float a)
{
	return mFontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	return mFontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const Vector4& color)
{
	return mFontManager->FindFontColor(color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const unsigned int color)
{
	return mFontManager->FindFontColor(color);
}

unsigned int CResourceManager::CreateFontColorKey(const float r, const float g, const float b, const float a)
{
	return mFontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	return mFontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(const Vector4& color)
{
	return mFontManager->CreateFontColorKey(color);
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, const float width, const float height)
{
	return mFontManager->CreateTextLayout(text, font, width, height);
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* text, const std::string& fontName, const float width, const float height)
{
	return mFontManager->CreateTextLayout(text, fontName, width, height);
}

IDWriteFontCollection1* CResourceManager::FindFontFile(const std::string& name)
{
	return mFontManager->FindFontFile(name);
}

IDWriteTextFormat* CResourceManager::FindFont(const std::string& name)
{
	return mFontManager->FindFont(name);
}

bool CResourceManager::CreateParticle(const std::string& name)
{
	return mParticleManager->CreateParticle(name);
}

CParticle* CResourceManager::FindParticle(const std::string& name)
{
	return mParticleManager->FindParticle(name);
}

void CResourceManager::ReleaseParticle(const std::string& name)
{
	mParticleManager->ReleaseParticle(name);
}

bool CResourceManager::CreateCSV(const std::string& name)
{
	return mExcelManager->CreateCSV(name);
}

CExcelData* CResourceManager::FindCSV(const std::string& name)
{
	return mExcelManager->FindCSV(name);
}

bool CResourceManager::DeleteCSV(const std::string& name)
{
	return mExcelManager->DeleteCSV(name);
}

bool CResourceManager::SaveCSV(const std::string& name, const char* fileName, const std::string& pathName)
{
	return mExcelManager->SaveCSV(name, fileName, pathName);
}

bool CResourceManager::SaveCSVFullPath(const std::string& name, const char* fullPath)
{
	return mExcelManager->SaveCSVFullPath(name, fullPath);
}

bool CResourceManager::LoadCSV(const char* fileName, const std::string& pathName)
{
	return mExcelManager->LoadCSV(fileName, pathName);
}

bool CResourceManager::LoadCSVFullPath(const char* fullPath)
{
	return mExcelManager->LoadCSVFullPath(fullPath);
}



