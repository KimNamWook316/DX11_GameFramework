#include "ExcelData.h"

CExcelData::CExcelData()
{
}

CExcelData::~CExcelData()
{
}

bool CExcelData::SaveCSV(const std::vector<std::vector<std::string>>& data, const std::string& name, const char* fileName, const std::string& pathName)
{
	return false;
}

bool CExcelData::SaveCSVFullPath(const std::vector<std::vector<std::string>>& data, const std::string& name, const char* fullPath)
{
	return false;
}

bool CExcelData::LoadCSV(const char* fileName, const std::string& pathName)
{
	return false;
}

bool CExcelData::LoadCSVFullPath(const char* fullPath)
{
	return false;
}
