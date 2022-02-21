#pragma once

#include "../../Ref.h"
#include "../Texture/Texture.h"
#include "../Material/Material.h"

class CTileSet :
    public CRef
{
    friend class CTileSetManager;

private:
    CTileSet();
    CTileSet(const CTileSet& tileSet);
    ~CTileSet();

public:
    bool Init();
    void Render();

public:
    TileSetInfo* FindInfo(const std::string& name);
    bool DeleteInfo(const std::string& name);
    void ClearTileInfo();
    void ClearTexture();
    void AddTileSetInfo(const std::string& name, const eTileShape& shape, const eTileType& type,
        const Vector2& start, const Vector2& end);
    bool RenameTileSetInfo(const std::string& name, const std::string& changeName);
    bool ChangeType(const std::string& name, eTileType& type);
    bool ChangeShape(const std::string& name, eTileShape& shape);
    bool ChangeImageStart(const std::string& name, const Vector2& start);
    bool ChangeImageEnd(const std::string& name, const Vector2& end);

public:
	void AddTileTexture(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddTileTextureFullPath(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fullPath);

	void SetTileTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTileTextureFullPath(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fullPath);

public:
    void Save(FILE* fp);
    void Load(FILE* fp);
    bool SaveCSV(const char* fileName, const std::string& pathName = EXCEL_PATH);
    bool SaveCSVFullPath(const char* fullPath);
    bool LoadCSV(const char* fileName, const std::string& pathName = EXCEL_PATH);
    bool LoadCSVFullPath(const char* fullPath);

public:
    CTexture* GetTexture()
    {
        return mTileMaterial->GetTexture();
    }

    void GetTileNames(std::vector<std::string>& outNames);

public:
    void SetScene(class CScene* scene)
    {
        mScene = scene;
    }

    void SetMaterial(CMaterial* material);

private:
    class CScene* mScene;
    CSharedPtr<CMaterial> mTileMaterial;
    char mCSVFullPath[MAX_PATH];
    char mTextureFullPath[MAX_PATH];
    std::unordered_map<std::string, TileSetInfo*> mMapTileInfo;
};

