#pragma once

#include "GameInfo.h"
#include "EditorInfo.h"

class CEditorUtil
{
public:
    static std::vector<std::string> GetFilesInDirectory(const std::string& path, const std::string& filter);
};

