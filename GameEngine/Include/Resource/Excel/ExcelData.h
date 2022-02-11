#pragma once

#include "../../GameInfo.h"

class CExcelData
{
	friend class CExcelManager;

private:
	CExcelData();
	~CExcelData();

public:
	bool Init();
	void SetLabel(const std::vector<std::string>& labels);
	void AddLabel(const std::string& label);
	void SetData(const std::string& name, const std::vector<std::string>& data);
	void SetData(const std::string& name, const std::string& label, const std::string& data);
	void SetData(const std::string& name, const std::string& label, int data);
	void SetData(const std::string& name, const std::string& label, float data);
	void SetData(const std::string& name, const std::string& label, bool data);
	void Clear();

public:
	bool SaveCSV(const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool SaveCSVFullPath(const char* fullPath);
	bool LoadCSV(const char* fileName, const std::string& pathName = EXCEL_PATH);
	bool LoadCSVFullPath(const char* fullPath);

public:
	const std::string& GetName() const
	{
		return mInfo.Name;
	}

	const std::vector<std::string>& GetLabels() const
	{
		return mInfo.Labels;
	}

	std::vector<std::string>* FindData(const std::string& name);

public:
	void SetName(const std::string& name)
	{
		mInfo.Name = name;
	}

private:
	int getLabelIndex(const std::string& label);
		
private:
	ExcelInfo mInfo;
};

