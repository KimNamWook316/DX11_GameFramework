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
	// 폰트 파일로부터 폰트를 로딩한다.
	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);
	// 실제 폰트 객체를 로딩한다.
	bool LoadFont(const std::string& name, const TCHAR* fontName, const int weight, const float fontSize,
		const TCHAR* localName, const int stretch = DWRITE_FONT_STRETCH_NORMAL);
	// 폰트 이름을 받아온다.
	const TCHAR* GetFontFaceName(const std::string& name);
	const char* GetFontFaceNameMultybyte(const std::string& name);
	bool CreateFontColor(const float r, const float g, const float b, const float a);
	bool CreateFontColor(const unsigned char r,const unsigned char g,const unsigned char b,const unsigned char a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(const unsigned int color);
	ID2D1SolidColorBrush* FindFontColor(const float r, const float g, const float b, const float a);
	ID2D1SolidColorBrush* FindFontColor(const unsigned char r,const unsigned char g,const unsigned char b,const unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(const unsigned int color);
	unsigned int CreateFontColorKey(const float r, const float g, const float b, const float a);
	unsigned int CreateFontColorKey(const unsigned char r,const unsigned char g,const unsigned char b,const unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& color);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, const float width, const float height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, const std::string& fontName, const float width, const float height);
	IDWriteFontCollection1* FindFontFile(const std::string& name);
	IDWriteTextFormat* FindFont(const std::string& name);

private:
	IDWriteFactory5* mWriteFactory;												// 모든 Dwrite 오브젝트에 대한 Root Factory interface
	std::unordered_map<std::string, IDWriteFontCollection1*> mMapFontFile;		// 폰트 파일로부터 가져온 폰트 리소스(?)
	std::unordered_map<std::string, IDWriteTextFormat*> mMapFont;				// Dwrite에서의 폰트 객체
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*> mMapFontColor;
	TCHAR	mFontFamilyName[256];
	char	mFontFamilyNameMultibyte[256];
};

