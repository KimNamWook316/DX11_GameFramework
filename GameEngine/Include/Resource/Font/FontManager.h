#pragma once

#include "../../GameInfo.h"

class CFontManager
{
	friend class CResourceManager;

private:
	CFontManager();
	~CFontManager();

public:
	bool Init();
	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);

private:
	IDWriteFactory* mWriteFactory;
	std::unordered_map<std::string, IDWriteFontCollection1*> mMapFontFile;
	std::unordered_map<std::string, IDWriteTextFormat*> mMapFont;
	std::unordered_map<std::string, ID2D1SolidColorBrush*> mMapFontColor;
};

