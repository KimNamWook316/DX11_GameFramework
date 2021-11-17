#pragma once

#include "GameInfo.h"

struct PathInfo
{
	// TCHAR : �����ڵ� ���ڿ��� �����ϱ� ���� Ÿ���̴�.
	// ��, ������Ʈ�� MultiByte ���ڿ��� ����Ѵٸ� char Ÿ������ ���ǵȴ�.
	// �����ڵ��� ��� wchar_t�� ���ǵȴ�.
	TCHAR	Path[MAX_PATH];
	char	PathMultibyte[MAX_PATH];

	PathInfo()
		: Path{}
		, PathMultibyte{}
	{
	}
};

class CPathManager
{
public:
	bool Init();
	bool AddPath(const std::string& name, const TCHAR* Path,
		const std::string& baseName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& name);

	DECLARE_SINGLE(CPathManager)

private:
	std::unordered_map<std::string, PathInfo*> mMapPath;
};

