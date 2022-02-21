#pragma once

#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"
#include "Texture/TextureManager.h"
#include "Animation/AnimationManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"
#include "Particle/ParticleManager.h"
#include "Excel/ExcelManager.h"
#include "TileSet/TileSetManager.h"

// �پ��� ���ҽ����� ��� ������ �Ŵ��� (�޽�, ����� ��� ��� ���ҽ��� �� ��ü������ �����Ѵ�.)
// CResource��� Ŭ������ �����, ��� ���ҽ��� ��ӹް� �Ͽ�, �� �ϳ��� �����ϴ� ���� �ִ�.
// Ȥ�� Hash�� ���� �����ϴ� ����� �ִµ�
// ���� ��� ������ �þ ��� ��� ���ҽ��� �ϳ��� �ʿ� ����ֱ� ������ Ž���� �������� ����
// Hash Table�� ��� Ž�� ��Ȯ���� ���� �ڷᰣ�� �Ÿ��� ũ�� ��ƾ� �ϱ� ������ �޸𸮸� ũ�� �����ϰ� �ȴ�.
// ���� ���⼭�� ��� ���ҽ��� ���� �����Ѵ�.
// �̰��� �� �����̶�� �� ���� ������, ���ҽ� ������ ���� ���� ����� �����ϰ� �ؾ� �Ѵ�.
class CResourceManager
{
public:
	bool Init();

public: // ===================== Mesh =====================
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);


public: // ===================== Shader =====================
	class CShader* FindShader(const std::string& name);
	void ReleaseShader(const std::string& name);

	template <typename T>
	bool CreateShader(const std::string& name)
	{
		return mShaderManager->CreateShader<T>(name);
	}

	bool CreateConstantBuffer(const std::string& name, const int size, const int reg,
		const int eConstantBufferShaderType = (int)eBufferShaderTypeFlags::All);

	class CConstantBuffer* FindConstantBuffer(const std::string& name);

public: // ===================== Material =====================
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);

	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		return mMaterialManager->CreateMaterial<T>(name);
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		return mMaterialManager->CreateMaterialEmpty<T>();
	}

public: // ===================== Texture =====================
	class CTexture* FindTexture(const std::string& name);
	void ReleaseTexture(const std::string& name);

	bool LoadTexture(const std::string& name, const TCHAR* fileName, 
		const std::string& filePath = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, 
		const std::string& path = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);

public: // ===================== Sequence2D =====================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	void AddAnimationSequece2DFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddAnimationSequece2DFrame(const std::string& name, const float startX, const float startY,
		const float width, const float height);
	CAnimationSequence2D* FindAnimationSequece2D(const std::string& name);
	void ReleaseAnimationSequece2D(const std::string& name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const;

	bool SaveSequence2DFullPath(const std::string& name, const char* fullPath);
	bool LoadSequence2DFullPath(std::string& outName, const char* fullPath, class CScene* scene = nullptr);
	bool LoadSequence2DFullPath(const char* fullPath, class CScene* scene = nullptr);
	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName = ANIMATION_PATH);
	bool LoadSequence2D(const char* fileName, const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadSequence2D(std::string& outName, const char* fileName, const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);

public: // ===================== Sound =====================
	bool LoadSound(const std::string& channelGroupName, bool bLoop, const std::string& soundName,
		const char* fileName, const std::string& pathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& groupName);
	bool SetVolume(int volume);											// 0 ~ 100
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& soundName);
	bool SoundStop(const std::string& soundName);
	bool SoundPause(const std::string& soundName);
	bool SoundResume(const std::string& soundName);
	CSound* FindSound(const std::string& soundName);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& groupName);
	void ReleaseSound(const std::string& soundName);

public: // ===================== Font =====================
	// ��Ʈ ���Ϸκ��� ��Ʈ�� �ε��Ѵ�.
	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);
	// ���� ��Ʈ ��ü�� �ε��Ѵ�.
	bool LoadFont(const std::string& name, const TCHAR* fontName, const int weight, const float fontSize,
		const TCHAR* localName, const int stretch = DWRITE_FONT_STRETCH_NORMAL);
	// ��Ʈ �̸��� �޾ƿ´�.
	const TCHAR* GetFontFaceName(const std::string& name);
	const char* GetFontFaceNameMultybyte(const std::string& name);
	bool CreateFontColor(const float r, const float g, const float b, const float a);
	bool CreateFontColor(const unsigned char r,const unsigned char g,const unsigned char b,const unsigned char a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(const unsigned int color);
	ID2D1SolidColorBrush* FindFontColor(const float r, const float g, const float b, const float a);
	ID2D1SolidColorBrush* FindFontColor(const unsigned char r,const unsigned char g,const unsigned char b,const unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(const unsigned int color);
	unsigned int CreateFontColorKey(const float r, const float g, const float b, const float a);
	unsigned int CreateFontColorKey(const unsigned char r,const unsigned char g,const unsigned char b,const unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& color);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, const float width, const float height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, const std::string& fontName, const float width, const float height);
	IDWriteFontCollection1* FindFontFile(const std::string& name);
	IDWriteTextFormat* FindFont(const std::string& name);

public: // ===================== Particle =====================
	bool CreateParticle(const std::string& name);
	CParticle* FindParticle(const std::string& name);
	void ReleaseParticle(const std::string& name);

public: // ===================== Excel =====================
	bool CreateCSV(const std::string& name);
	CExcelData* FindCSV(const std::string& name);
	bool DeleteCSV(const std::string& name);
	bool SaveCSV(const std::string& name, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool SaveCSVFullPath(const std::string& name, const char* fullPath);
	bool LoadCSV(const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadCSVFullPath(const char* fullPath);

public: // ===================== Excel =====================
	bool CreateTileSet(const std::string& name);
	CTileSet* FindTileSet(const std::string& name);
	void ReleaseTileSet(const std::string& name);
	bool SaveTileSet(const std::string& name, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool SaveTileSetFullPath(const std::string& name, const char* fullPath);
	bool LoadTileSet(const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadTileSet(std::string& outName, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadTileSetFullPath(const char* fullPath);
	bool LoadTileSetFullPath(std::string& outName, const char* fullPath);

	DECLARE_SINGLE(CResourceManager)

private:
	CMeshManager* mMeshManager;
	CShaderManager* mShaderManager;
	CMaterialManager* mMaterialManager;
	CTextureManager* mTextureManager;
	CAnimationManager* mAnimationManager;
	CSoundManager* mSoundManager;
	CFontManager* mFontManager;
	CParticleManager* mParticleManager;
	CExcelManager* mExcelManager;
	CTileSetManager* mTileSetManager;
};

