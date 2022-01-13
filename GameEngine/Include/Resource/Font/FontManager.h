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
	// ��Ʈ ���Ϸκ��� ��Ʈ�� �ε��Ѵ�.
	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);
	// ���� ��Ʈ ��ü�� �ε��Ѵ�.
	bool LoadFont(const std::string& name, const TCHAR* fontName, const int weight, const float fontSize,
		const TCHAR* localName, const int stretch = DWRITE_FONT_STRETCH_NORMAL);
	// ��Ʈ �̸��� �޾ƿ´�.
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
	IDWriteFactory5* mWriteFactory;												// ��� Dwrite ������Ʈ�� ���� Root Factory interface
	std::unordered_map<std::string, IDWriteFontCollection1*> mMapFontFile;		// ��Ʈ ���Ϸκ��� ������ ��Ʈ ���ҽ�(?)
	std::unordered_map<std::string, IDWriteTextFormat*> mMapFont;				// Dwrite������ ��Ʈ ��ü
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*> mMapFontColor;
	TCHAR	mFontFamilyName[256];
	char	mFontFamilyNameMultibyte[256];
};

