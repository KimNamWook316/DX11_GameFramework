#include "ExcelManager.h"
#include "../../PathManager.h"

CExcelManager::CExcelManager()
{
}

CExcelManager::~CExcelManager()
{
	auto iter = mMapExcelData.begin();
	auto iterEnd = mMapExcelData.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mMapExcelData.clear();
}

bool CExcelManager::Init()
{
	return true;
}

CExcelData* CExcelManager::FindData(const std::string& name)
{
	auto iter = mMapExcelData.find(name);

	if (iter == mMapExcelData.end())
	{
		return nullptr;
	}
	return iter->second;
}

bool CExcelManager::DeleteData(const std::string& name)
{
	auto iter = mMapExcelData.find(name);

	if (iter == mMapExcelData.end())
	{
		return false;
	}
	
	SAFE_DELETE(iter->second);
	mMapExcelData.erase(iter);
	return true;
}

bool CExcelManager::SaveCSV(const std::string& name, const char* fileName, const std::string& pathName)
{
	CExcelData* excelData = FindData(name);

	if (excelData)
	{
		excelData = new CExcelData;
	}

	return excelData->SaveCSV(name, fileName, pathName);
}

bool CExcelManager::SaveCSVFullPath(const std::string& name, const char* fullPath)
{
	CExcelData* excelData = FindData(name);

	if (excelData)
	{
		excelData = new CExcelData;
	}

	return excelData->SaveCSVFullPath(name, fullPath);
}

bool CExcelManager::LoadCSV(const char* fileName, const std::string& pathName)
{
	// TODO : Fix
 //	CExcelData* data = FindData(name);
 //
 //	if (data)
 //	{
 //		return true;
 //	}
 //
 //	data = new CExcelData;
 //
 //	if (!data->LoadCSV(fileName, pathName))
 //	{
 //		SAFE_DELETE(data);
 //		return false;
 //	}
 //	
 //	mMapExcelData.insert(std::make_pair(name, data));
 	return true;
}

bool CExcelManager::LoadCSVFullPath(const char* fullPath)
{
	CExcelData* data = new CExcelData;

	if (!data->LoadCSVFullPath(fullPath))
	{
		SAFE_DELETE(data);
		return false;
	}
	
	std::string name = data->GetName();
	if (FindData(name))
	{
		SAFE_DELETE(data);
		return true;
	}
	
	mMapExcelData.insert(std::make_pair(name, data));
	return true;
}
