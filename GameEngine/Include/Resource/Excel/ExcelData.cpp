#include "ExcelData.h"
#include "../../PathManager.h"

CExcelData::CExcelData()	:
	mInfo{}
{
}

CExcelData::~CExcelData()
{
	auto iter = mInfo.Data.begin();
	auto iterEnd = mInfo.Data.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mInfo.Data.clear();
}

bool CExcelData::Init()
{
	return true;
}

void CExcelData::SetLabel(const std::vector<std::string>& label)
{
	mInfo.Labels.clear();
	mInfo.Labels = label;
}

void CExcelData::AddLabel(const std::string& label)
{
	size_t size = mInfo.Labels.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (label == mInfo.Labels[i])
		{
			return;
		}
	}

	mInfo.Labels.push_back(label);
}

void CExcelData::SetData(const std::string& name, const std::vector<std::string>& data)
{
	std::vector<std::string>* found = FindData(name);

	if (!found)
	{
		found = new std::vector<std::string>;
		mInfo.Data.insert(std::make_pair(name, found));
	}

	*found = data;
}

void CExcelData::SetData(const std::string& name, const std::string& label, const std::string& data)
{
	std::vector<std::string>* found = FindData(name);

	if (!found)
	{
		found = new std::vector<std::string>;
		found->resize(mInfo.Labels.size());
		mInfo.Data.insert(std::make_pair(name, found));
	}

	int labelIdx = getLabelIndex(label);
	if (-1 == labelIdx)
	{
		return;
	}

	(*found)[labelIdx] = data;
}

void CExcelData::SetData(const std::string& name, const std::string& label, int data)
{
	SetData(name, label, std::to_string(data));
}

void CExcelData::SetData(const std::string& name, const std::string& label, float data)
{
	SetData(name, label, std::to_string(data));
}

void CExcelData::SetData(const std::string& name, const std::string& label, bool data)
{
	std::string str = data ? "true" : "false";
	SetData(name, label, str);
}

void CExcelData::Clear()
{
	mInfo.Labels.clear();

	auto iter = mInfo.Data.begin();
	auto iterEnd = mInfo.Data.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mInfo.Data.clear();
}

bool CExcelData::SaveCSV(const std::string& name, const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
	{
		strcpy_s(fullPath, path->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	return SaveCSVFullPath(name, fullPath);
}

bool CExcelData::SaveCSVFullPath(const std::string& name, const char* fullPath)
{
 //	mInfo.Name = name;
 //
 //	FILE* fp = nullptr;
 //	fopen_s(&fp, fullPath, "wt");
 //
 //	if (!fp)
 //	{
 //		assert(false);
 //		return false;
 //	}
 //
 //	fputs(mInfo.Name.c_str(), fp);
 //
 //	size_t rowCount = mInfo.Sheet.size();
 //	for (size_t i = 0; i < rowCount; ++i)
 //	{
 //		char buf[1024] = {};
 //
 //		size_t colCount = mInfo.Sheet[i].size();
 //		for (size_t j = 0; j < colCount; ++j)
 //		{
 //			int length = (int)mInfo.Sheet[i][j].length();
 //			strcpy_s(buf, length, mInfo.Sheet[i][j].c_str());
 //
 //			if (j == colCount - 1)
 //			{
 //				strcat_s(buf, "\n");
 //			}
 //			else
 //			{
 //				strcat_s(buf, ",");
 //			}
 //		}
 //		fputs(buf, fp);
 //	}
 //
 //	fclose(fp);
	return true;
}

bool CExcelData::LoadCSV(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
	{
		strcpy_s(fullPath, path->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	return LoadCSVFullPath(fullPath);
}

bool CExcelData::LoadCSVFullPath(const char* fullPath)
{
 //	FILE* fp = nullptr;
 //
 //	fopen_s(&fp, fullPath, "rt");
 //
 //	if (!fp)
 //	{
 //		assert(false);
 //		return false;
 //	}
 //
 //	char name[64] = {};
 //	fgets(name, 64, fp);
 //	mInfo.Name = name;
 //
 //	char buf[1024] = {};
 //
 //	while (0 == feof(fp))
 //	{
 //		std::vector<std::string> row;
 //		fgets(buf, 1024, fp);
 //
 //		char* context = nullptr;
 //		char* elementBuf = strtok_s(buf, ",", &context);
 //
 //		while (elementBuf != NULL)
 //		{
 //			row.push_back(elementBuf);
 //			elementBuf = strtok_s(NULL, ",", &context);
 //		}
 //		
 //		mInfo.Sheet.push_back(row);
 //	}
 //	
 //	fclose(fp);
 	return true;
}

std::vector<std::string>* CExcelData::FindData(const std::string& name)
{
	auto iter = mInfo.Data.find(name);
	
	if (iter == mInfo.Data.end())
	{
		return nullptr;
	}

	return iter->second;
}

int CExcelData::getLabelIndex(const std::string& label)
{
	int idx = 0;
	int size = (int)mInfo.Labels.size();

	for (; idx < size; ++idx)
	{
		if (label == mInfo.Labels[idx])
		{
			return idx;
		}
	}

	return -1;
}
