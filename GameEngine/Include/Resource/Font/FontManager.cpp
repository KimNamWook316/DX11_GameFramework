#include "FontManager.h"

CFontManager::CFontManager()	:
	mWriteFactory(nullptr)
{
}

CFontManager::~CFontManager()
{
	SAFE_RELEASE(mWriteFactory);
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(mWriteFactory),
		(IUnknown**)&mWriteFactory)))
	{
		assert(false);
		return false;
	}
	return true;
}

bool CFontManager::CreateFontFile(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return false;
}
