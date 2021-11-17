#pragma once

#include "GameInfo.h"

struct PathInfo
{
	// TCHAR : 유니코드 문자열을 저장하기 위한 타입이다.
	// 단, 프로젝트가 MultiByte 문자열을 사용한다면 char 타입으로 정의된다.
	// 유니코드일 경우 wchar_t로 정의된다.
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

