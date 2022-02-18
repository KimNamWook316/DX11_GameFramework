#pragma once

#include "../../Ref.h"
#include "../Texture/Texture.h"

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

public:
    TileSetInfo* FindInfo(const std::string& name);
    bool DeleteInfo(const std::string& name);
    void AddTileSetInfo(const std::string& name, const eTileShape& shape, const eTileType& type,
        const Vector2& start, const Vector2& end);
    bool RenameTileSetInfo(const std::string& name, const std::string& changeName);
    bool ChangeType(const std::string& name, eTileType& type);
    bool ChangeShape(const std::string& name, eTileShape& shape);
    bool ChangeImageStart(const std::string& name, const Vector2& start);
    bool ChangeImageEnd(const std::string& name, const Vector2& end);

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
        return mTextureAtlas;
    }

    void GetTileNames(std::vector<std::string>& outNames);

public:
    void SetTexture(CTexture* texture)
    {
        mTextureAtlas = texture;
    }

    void SetScene(class CScene* scene)
    {
        mScene = scene;
    }

private:
    class CScene* mScene;
    CSharedPtr<CTexture> mTextureAtlas;
    char mCSVFullPath[MAX_PATH];
    std::unordered_map<std::string, TileSetInfo*> mMapTileInfo;
};

