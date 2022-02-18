#pragma once

#include "TileSet.h"

class CTileSetManager
{
	friend class CResourceManager;

private:
	CTileSetManager();
	~CTileSetManager();

public:
	bool Init();
	bool CreateTileSet(const std::string& name);
	CTileSet* FindTileSet(const std::string& name);
	void ReleaseTileSet(const std::string& name);

public:
	bool SaveTileSet(const std::string& name, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool SaveTileSetFullPath(const std::string& name, const char* fullPath);
	bool LoadTileSet(const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadTileSet(std::string& outName, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadTileSetFullPath(const char* fullPath);
	bool LoadTileSetFullPath(std::string& outName, const char* fullPath);

private:
	std::unordered_map<std::string, CSharedPtr<CTileSet>> mMapTileSet;
};

