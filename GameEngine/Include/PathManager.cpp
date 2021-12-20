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
    // RootPath�� ��θ� ���´�.
    TCHAR   Path[MAX_PATH] = {};

    // GetModuleFileName : ���� ���������� �ִ� ��� + ���������̸�.exe ��
    // ��ü��θ� ������ �ȴ�.
    // ���⿡�� �������� �̸������� ���� Bin���� ������ ���ͼ�
    // Root������ ����Ѵ�.
    GetModuleFileName(0, Path, MAX_PATH);

    // ���� ���ڿ��� �ڿ������� ���ʷ� �����Ͽ� \�� / �� �ִ� ���ڸ� ã�´�.
    int length = lstrlen(Path);

    // blahblah~\Bin\GameFramework_Debug.exe
    for (int i = length - 1; i > 0; --i)
    {
        if ('\\' == Path[i])
        {
            // �������ϸ���� ������ �߶󳽴�.
            memset(&Path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
            break;
        }
    }

    PathInfo* pInfo = new PathInfo;

    lstrcpy(pInfo->Path, Path);

#ifdef UNICODE
    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.

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
    // �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
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
