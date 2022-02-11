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
	std::string str = data ? "TRUE" : "FALSE";
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

bool CExcelData::SaveCSV(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
	{
		strcpy_s(fullPath, path->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	return SaveCSVFullPath(fullPath);
}

bool CExcelData::SaveCSVFullPath(const char* fullPath)
{
	FILE* fp = nullptr;
	fopen_s(&fp, fullPath, "wt");

	if (!fp)
	{
		assert(false);
		return false;
	}

	char buf[1024] = {};

	// Name
	fputs(mInfo.Name.c_str(), fp);
	fputs("\n", fp);
	
	// Label
	fputs(",", fp);
	size_t labelCount = mInfo.Labels.size();
	for (size_t i = 0; i < labelCount; ++i)
	{
		int length = mInfo.Labels[i].length();
		strcpy_s(buf, length + 1, mInfo.Labels[i].c_str());
		if (i == labelCount - 1)
		{
			strcat_s(buf, "\n");
		}
		else
		{
			strcat_s(buf, ",");
		}
		fputs(buf, fp);
	}

	// Data
	auto iter = mInfo.Data.begin();
	auto iterEnd = mInfo.Data.end();

	int count = 0;
	int rowCount = (int)mInfo.Data.size();
	for (; iter != iterEnd; ++iter)
	{
		int length = iter->first.length();
		strcpy_s(buf, length + 1, iter->first.c_str());
		strcat_s(buf, ",");
		fputs(buf, fp);
		
		size_t dataCount = iter->second->size();
		for (size_t i = 0; i < dataCount; ++i)
		{
			std::string data = (*(iter->second))[i];
			length = data.length();
			strcpy_s(buf, length + 1, data.c_str());

			if (i == dataCount - 1 && count != rowCount - 2)
			{
				strcat_s(buf, "\n");
			}
			else
			{
				strcat_s(buf, ",");
			}

			fputs(buf, fp);
		}
		++rowCount;
	}
	
	fclose(fp);
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
	FILE* fp = nullptr;

	fopen_s(&fp, fullPath, "rt");

	if (!fp)
	{
		assert(false);
		return false;
	}

	// name
	char name[64] = {};
	fgets(name, 64, fp);
	int len = strlen(name);
	name[len - 2] = '\0'; // , 제거
	mInfo.Name = name;

	// label
	char buf[1024] = {};
	fgets(buf, 1024, fp);
	char* context = nullptr;
	char* element = strtok_s(buf, ",", &context);
	while (!element)
	{
		// 처음 잘린 문자는 무시한다. 맨 앞이 ,이기 때문에
		element = strtok_s(NULL, ",", &context);
		mInfo.Labels.push_back(element);
	}

	// data
	std::string dataName;
	std::vector<std::string>* row = nullptr;
	while (0 == feof(fp))
	{
		row = new std::vector<std::string>;
		fgets(buf, 1024, fp);
		element = strtok_s(buf, ",", &context);
		dataName = element;
		
		while (!element)
		{
			element = strtok_s(NULL, ",", &context);
			row->push_back(element);
		}

		mInfo.Data.insert(std::make_pair(dataName, row));
	}
	
	fclose(fp);
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
