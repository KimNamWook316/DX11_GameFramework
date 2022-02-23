#include "ExcelData.h"
#include "../../PathManager.h"

CExcelData::CExcelData()	:
	mInfo{}
{
}

CExcelData::~CExcelData()
{
	{
		auto iter = mInfo.Data.begin();
		auto iterEnd = mInfo.Data.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		mInfo.Data.clear();
	}
	{
		size_t size = mInfo.PushOrderedData.size();

		for (size_t i = 0; i < size; ++i)
		{
			SAFE_DELETE(mInfo.PushOrderedData[i]);
		}

		mInfo.PushOrderedData.clear();
	}
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

void CExcelData::AddRow(const std::string& rowName)
{
	std::vector<std::string>* found = GetRow(rowName);

	if (found)
	{
		return;
	}

	found = new std::vector<std::string>;
	found->resize(mInfo.Labels.size());
	mInfo.Data.insert(std::make_pair(rowName, found));
	
	std::vector<std::string>* orderedData = new std::vector<std::string>;
	orderedData->push_back(rowName);
	orderedData->resize(mInfo.Labels.size() + 1);
	mInfo.PushOrderedData.push_back(orderedData);
}

void CExcelData::SetData(const std::string& name, const std::vector<std::string>& data)
{
	std::vector<std::string>* found = GetRow(name);

	if (!found)
	{
		found = new std::vector<std::string>;
		mInfo.Data.insert(std::make_pair(name, found));

		// Ordered Data 에는 Row의 Name도 들어간다.
		std::vector<std::string>* orderedData = new std::vector<std::string>;
		orderedData->push_back(name);
		
		mInfo.PushOrderedData.push_back(orderedData);
	}

	found->clear();

	size_t size = data.size();
	for (size_t i = 0; i < size; ++i)
	{
		// 빈 셀일 경우 공백을 넣어준다.
		if (0 == data[i].length())
		{
			found->push_back(" ");
		}
		else
		{
			found->push_back(data[i]);
		}
	}

	std::vector<std::string>* orderedData = findOrderedData(name);

	if (orderedData->size() < size + 1)
	{
		orderedData->resize(size + 1);
	}

	for (size_t i = 1; i < size + 1; ++i)
	{
		(*orderedData)[i] = data[i - 1];
	}
}

void CExcelData::SetData(const std::string& name, const std::string& label, const std::string& data)
{
	std::vector<std::string>* found = GetRow(name);

	if (!found)
	{
		found = new std::vector<std::string>;
		found->resize(mInfo.Labels.size());

		// 공백을 넣어 빈 셀로 만든다.
		size_t size = found->size();
		for (size_t i = 0; i < size; ++i)
		{
			(*found)[i] = std::string(" ");
		}
		mInfo.Data.insert(std::make_pair(name, found));

		std::vector<std::string>* orderedData = new std::vector<std::string>;
		orderedData->push_back(name);

		size = found->size();
		for (size_t i = 0; i < size; ++i)
		{
			orderedData->push_back((*found)[i]);
		}
		
		mInfo.PushOrderedData.push_back(orderedData);
	}

	int labelIdx = getLabelIndex(label);
	if (-1 == labelIdx)
	{
		return;
	}

	(*found)[labelIdx] = data;

	std::vector<std::string>* orderedData = findOrderedData(name);
	(*orderedData)[labelIdx + 1] = data;
}

void CExcelData::SetData(const std::string& name, const std::string& label, const char* data)
{
	std::string str = data;
	SetData(name, label, str);
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

bool CExcelData::DeleteRow(const std::string& name)
{
	auto iter = mInfo.Data.find(name);
	
	if (iter == mInfo.Data.end())
	{
		return false;
	}

	SAFE_DELETE(iter->second);
	mInfo.Data.erase(iter);

	auto iterVec = mInfo.PushOrderedData.begin();
	auto iterVecEnd = mInfo.PushOrderedData.end();

	for (; iterVec != iterVecEnd; ++iterVec)
	{
		if ((**iterVec)[0] == name)
		{
			SAFE_DELETE(*iterVec);
			mInfo.PushOrderedData.erase(iterVec);
			break;
		}
	}

	return true;
}

bool CExcelData::RenameRow(const std::string& prevName, const std::string& changeName)
{
	auto iter = mInfo.Data.find(prevName);
	
	if (iter == mInfo.Data.end())
	{
		return false;
	}

	std::vector<std::string>* data = iter->second;
	mInfo.Data.erase(iter);
	mInfo.Data.insert(std::make_pair(changeName, data));

	auto iterVec = mInfo.PushOrderedData.begin();
	auto iterVecEnd = mInfo.PushOrderedData.end();

	for (; iterVec != iterVecEnd; ++iterVec)
	{
		if ((**iterVec)[0] == prevName)
		{
			(**iterVec)[0] = changeName;
			break;
		}
	}

	return true;
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
	
	size_t size = mInfo.PushOrderedData.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mInfo.PushOrderedData[i]);
	}

	mInfo.PushOrderedData.clear();
}

void CExcelData::ClearData()
{	
	auto iter = mInfo.Data.begin();
	auto iterEnd = mInfo.Data.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mInfo.Data.clear();	
	
	size_t size = mInfo.PushOrderedData.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mInfo.PushOrderedData[i]);
	}

	mInfo.PushOrderedData.clear();
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
	size_t colSize = mInfo.PushOrderedData.size();
	for (size_t i = 0; i < colSize; ++i)
	{
		std::vector<std::string>* row = mInfo.PushOrderedData[i];
		
		size_t rowSize = row->size();

		for (size_t j = 0; j < rowSize; ++j)
		{
			int length = (*row)[j].length();
			strcpy_s(buf, length + 1, (*row)[j].c_str());

			if (j == rowSize - 1)
			{
				strcat_s(buf, "\n");
			}
			else
			{
				strcat_s(buf, ",");
			}
			fputs(buf, fp);
		}
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
	name[len - 1] = '\0'; // , 제거
	mInfo.Name = name;

	// label
	char buf[1024] = {};
	fgets(buf, 1024, fp);
	char* context = nullptr;
	char* element = strtok_s(buf, ",", &context);
	while (NULL != element)
	{
		mInfo.Labels.push_back(element);
		element = strtok_s(NULL, ",", &context);
	}
	// 공백 제거
	size_t labelSize = mInfo.Labels.size();
	mInfo.Labels[labelSize - 1].erase(std::remove(mInfo.Labels[labelSize - 1].begin(), mInfo.Labels[labelSize - 1].end(), '\n'),
		mInfo.Labels[labelSize-1].end());

	// data
	std::string dataName;
	std::vector<std::string>* row = nullptr;
	std::vector<std::string>* rowOrdered = nullptr;
	while (0 == feof(fp))
	{
		row = new std::vector<std::string>;
		rowOrdered = new std::vector<std::string>;

		fgets(buf, 1024, fp);
		element = strtok_s(buf, ",", &context);
		dataName = element;
		rowOrdered->push_back(element);
		
		while (NULL != element)
		{
			if (strcmp("\n", element) == 0)
			{
				break;
			}

			element = strtok_s(NULL, ",", &context);

			if (!element)
			{
				break;
			}
			row->push_back(element);
			rowOrdered->push_back(element);
		}

		if (row->size() == 0)
		{
			SAFE_DELETE(row);
			SAFE_DELETE(rowOrdered);
			break;
		}

		// 공백 제거
		size_t labelSize = row->size();
		(*row)[labelSize - 1].erase(std::remove((*row)[labelSize - 1].begin(), (*row)[labelSize - 1].end(), '\n'),
		(*row)[labelSize-1].end());
		labelSize = rowOrdered->size();
		(*rowOrdered)[labelSize - 1].erase(std::remove((*rowOrdered)[labelSize - 1].begin(), (*rowOrdered)[labelSize - 1].end(), '\n'),
		(*rowOrdered)[labelSize-1].end());

		mInfo.Data.insert(std::make_pair(dataName, row));
		mInfo.PushOrderedData.push_back(rowOrdered);
	}
	
	fclose(fp);
	return true;
}

void CExcelData::GetRowNames(std::vector<std::string>& outNames) const
{
	size_t size = mInfo.PushOrderedData.size();

	for (size_t i = 0; i < size; ++i)
	{
		outNames.push_back((*mInfo.PushOrderedData[i])[0]);
	}
}

// TODO : 포인터 return
const std::unordered_map<std::string, std::vector<std::string>*> CExcelData::GetRowWithName(const int idx)
{
	std::unordered_map<std::string, std::vector<std::string>*> out;

	if (idx < 0 || idx >= mInfo.Data.size())
	{
		assert(false);
		return out;
	}

	int count = 0;
	
	auto iter = mInfo.Data.begin();
	auto iterEnd = mInfo.Data.end();

	for (; iter != iterEnd; ++iter)
	{
		if (idx == count)
		{
			out.insert(std::make_pair(iter->first, iter->second));
			return out;
		}
		++count;
	}
}

std::vector<std::string>* CExcelData::GetRow(const std::string& name)
{
	auto iter = mInfo.Data.find(name);
	
	if (iter == mInfo.Data.end())
	{
		return nullptr;
	}

	return iter->second;
}

const std::string& CExcelData::FindData(const std::string& name, const std::string& label)
{
	std::vector<std::string>* row = GetRow(name);
	int idx = getLabelIndex(label);

	return (*row)[idx];
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

std::vector<std::string>* CExcelData::findOrderedData(const std::string& name)
{
	size_t size = mInfo.PushOrderedData.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (name == (*mInfo.PushOrderedData[i])[0])
		{
			return mInfo.PushOrderedData[i];
		}
	}
	return nullptr;
}
