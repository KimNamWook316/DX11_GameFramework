#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
    auto iter = mMapPath.begin();
    auto iterEnd = mMapPath.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }

    mMapPath.clear();
}

bool CPathManager::Init()
{
    // RootPath의 경로를 얻어온다.
    TCHAR   Path[MAX_PATH] = {};

    // GetModuleFileName : 현재 실행파일이 있는 경로 + 실행파일이름.exe 로
    // 전체경로를 얻어오게 된다.
    // 여기에서 실행파일 이름까지를 빼서 Bin폴더 까지만 얻어와서
    // Root폴더로 사용한다.
    GetModuleFileName(0, Path, MAX_PATH);

    // 얻어온 문자열을 뒤에서부터 차례로 접근하여 \나 / 가 있는 문자를 찾는다.
    int length = lstrlen(Path);

    // blahblah~\Bin\GameFramework_Debug.exe
    for (int i = length - 1; i > 0; --i)
    {
        if ('\\' == Path[i])
        {
            // 실행파일명부터 끝까지 잘라낸다.
            memset(&Path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
            break;
        }
    }

    PathInfo* pInfo = new PathInfo;

    lstrcpy(pInfo->Path, Path);

#ifdef UNICODE
    // 유니코드 문자열을 멀티바이트 문자열로 변환한다.

    int convertLength = WideCharToMultiByte(CP_ACP, 0, Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, Path, -1,
        pInfo->PathMultibyte, convertLength, 0, 0);
#else
    strcpy_s(info->PathMultibyte, Path);
#endif
    mMapPath.insert(std::make_pair(ROOT_PATH, pInfo));

    AddPath(SHADER_PATH, TEXT("Shader\\"));
    AddPath(TEXTURE_PATH, TEXT("Texture\\"));
    AddPath(FONT_PATH, TEXT("Font\\"));
    AddPath(ANIMATION_PATH, TEXT("Animation\\"));
    AddPath(SCENE_PATH, TEXT("Scene\\"));

    return true;
}

bool CPathManager::AddPath(const std::string& name, const TCHAR* Path, const std::string& baseName)
{
    if (FindPath(name))
    {
        return false;
    }

    const PathInfo* pBaseInfo = FindPath(baseName);

    if (!pBaseInfo)
    {
        return false;
    }

    PathInfo* pInfo = new PathInfo;

    lstrcpy(pInfo->Path, pBaseInfo->Path);
    lstrcat(pInfo->Path, Path);

#ifdef UNICODE
    // 유니코드 문자열을 멀티바이트 문자열로 변환한다.
    int convertLength = WideCharToMultiByte(CP_ACP, 0, pInfo->Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, pInfo->Path, -1,
        pInfo->PathMultibyte, convertLength, 0, 0);
#else
    strcpy_s(info->PathMultibyte, Path);
#endif
    mMapPath.insert(std::make_pair(name, pInfo));
    
    return true;
}

const PathInfo* CPathManager::FindPath(const std::string& name)
{
    std::unordered_map<std::string, PathInfo*>::iterator iter = mMapPath.find(name);

    if (iter == mMapPath.end())
    {
        return nullptr;
    }

    return iter->second;
}
