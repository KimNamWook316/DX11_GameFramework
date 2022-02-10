#pragma once

#include "../../GameInfo.h"

class CExcelData
{
	friend class CExcelManager;

private:
	CExcelData();
	~CExcelData();

public:
	bool SaveCSV(const std::vector<std::vector<std::string>>& data, const std::string& name, const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool SaveCSVFullPath(const std::vector<std::vector<std::string>>& data, const std::string& name, const char* fullPath);
	bool LoadCSV(const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadCSVFullPath(const char* fullPath);

public:
	const std::string& GetName() const
	{
		return mName;
	}

	const std::vector<std::vector<std::string>>& GetData() const
	{
		return mData;
	}

private:
	std::string mName;
	std::vector<std::vector<std::string>> mData;
};

