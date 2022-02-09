#pragma once

#include "ExcelData.h"

class CExcelManager
{
	friend class CResourceManager;

private:
	CExcelManager();
	~CExcelManager();

public:
	bool Init();

public:
	CExcelData* FindData(const std::string& name);
	bool DeleteData(const std::string& name);

public:
	bool SaveCSV(const std::vector<std::vector<std::string>>& data, const std::string& name, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool SaveCSVFullPath(const std::vector<std::vector<std::string>>& data, const std::string& name, const char* fullPath);
	bool LoadCSV(const std::string& name, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadCSVFullPath(const char* fullPath);

private:
	std::unordered_map<std::string, class CExcelData*> mMapExcelData;
};

