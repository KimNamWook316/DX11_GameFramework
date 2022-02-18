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

// 다양한 리소스들을 모두 관리할 매니저 (메쉬, 오디오 등등 모든 리소스는 이 객체에서만 관리한다.)
// CResource라는 클래스를 만들어, 모든 리소스가 상속받게 하여, 맵 하나로 관리하는 법도 있다.
// 혹은 Hash를 통해 관리하는 방법도 있는데
// 맵의 경우 개수가 늘어날 경우 모든 리소스가 하나의 맵에 들어있기 때문에 탐색이 느려지는 문제
// Hash Table의 경우 탐색 정확성을 위해 자료간의 거리를 크게 잡아야 하기 때문에 메모리를 크게 차지하게 된다.
// 따라서 여기서는 모든 리소스를 따로 관리한다.
// 이것이 꼭 정답이라고 할 수는 없으며, 리소스 개수에 따라 관리 방식을 유연하게 해야 한다.
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
	// 폰트 파일로부터 폰트를 로딩한다.
	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);
	// 실제 폰트 객체를 로딩한다.
	bool LoadFont(const std::string& name, const TCHAR* fontName, const int weight, const float fontSize,
		const TCHAR* localName, const int stretch = DWRITE_FONT_STRETCH_NORMAL);
	// 폰트 이름을 받아온다.
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

