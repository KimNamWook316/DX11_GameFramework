#include "FontManager.h"
#include "../../PathManager.h"
#include "../../Device.h"

CFontManager::CFontManager()	:
	mWriteFactory(nullptr),
	mFontFamilyName{},
	mFontFamilyNameMultibyte{}
{
}

CFontManager::~CFontManager()
{
	{
		auto iter = mMapFontFile.begin();
		auto iterEnd = mMapFontFile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto iter = mMapFont.begin();
		auto iterEnd = mMapFont.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto iter = mMapFontColor.begin();
		auto iterEnd = mMapFontColor.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

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

	// Load Engine Default Font
	CreateFontFile("Default", TEXT("NotoSansKR-Regular.otf"));
	
	const TCHAR* fontFace = GetFontFaceName("Default");

	LoadFont("Default", fontFace, 600, 20.f, TEXT("ko"));

	return true;
}

bool CFontManager::CreateFontFile(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	IDWriteFontCollection1* fontCollection = FindFontFile(name);

	if (fontCollection)
	{
		return true;
	}

	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		lstrcpy(fullPath, info->Path);
	}
	lstrcat(fullPath, fileName);

	IDWriteFontFile* fontFile = nullptr;	// FontFile Reference를 얻기 위해 필요한 객체
	
	if (FAILED(mWriteFactory->CreateFontFileReference(fullPath, nullptr, &fontFile)))
	{
		assert(false);
		return false;
	}

	IDWriteFontSetBuilder1* builder = nullptr; // FontFile을 추가하여 FontSet을 만드는 Builder 객체

	if (FAILED(mWriteFactory->CreateFontSetBuilder(&builder)))
	{
		assert(false);
		return false;
	}

	builder->AddFontFile(fontFile);

	IDWriteFontSet* fontSet = nullptr;	// FontCollection과 바꿔먹을 것

	if (FAILED(builder->CreateFontSet(&fontSet)))
	{
		assert(false);
		return false;
	}

	if (FAILED(mWriteFactory->CreateFontCollectionFromFontSet(fontSet, &fontCollection)))
	{
		assert(false);
		return false;
	}

	mMapFontFile.insert(std::make_pair(name, fontCollection));

	SAFE_RELEASE(fontSet);
	SAFE_RELEASE(builder);
	SAFE_RELEASE(fontFile);

	return true;
}

bool CFontManager::LoadFont(const std::string& name, const TCHAR* fontName, const int weight, const float fontSize, const TCHAR* localName, const int stretch)
{
	IDWriteTextFormat* font = FindFont(name);

	if (font)
	{
		return true;
	}

	if (FAILED(mWriteFactory->CreateTextFormat(fontName, nullptr, (DWRITE_FONT_WEIGHT)weight, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, fontSize, localName, &font)))
	{
		assert(false);
		return false;
	}

	mMapFont.insert(std::make_pair(name, font));
	return true;
}

const TCHAR* CFontManager::GetFontFaceName(const std::string& name)
{
	IDWriteFontCollection1* fontCollection = FindFontFile(name);

	if (!fontCollection)
	{
		return nullptr;
	}

	IDWriteFontFamily* family = nullptr;

	if (FAILED(fontCollection->GetFontFamily(0, &family)))
	{
		assert(false);
		return nullptr;
	}

	IDWriteLocalizedStrings* localizedName = nullptr;

	if (FAILED(family->GetFamilyNames(&localizedName)))
	{
		assert(false);
		return nullptr;
	}

	memset(mFontFamilyName, 0, sizeof(TCHAR) * 256);

	if (FAILED(localizedName->GetString(0, mFontFamilyName, 256)))
	{
		assert(false);
		return nullptr;
	}

	return mFontFamilyName;
}

const char* CFontManager::GetFontFaceNameMultybyte(const std::string& name)
{
	if (!GetFontFaceName(name))
	{
		return nullptr;
	}

	memset(mFontFamilyNameMultibyte, 0, sizeof(char) * 256);

#ifdef UNICODE
	int length = WideCharToMultiByte(CP_ACP, 0, mFontFamilyName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, mFontFamilyName, -1, mFontFamilyNameMultibyte, length, 0, 0);
#else
	strcpy_s(mFontFamilyNameMultibyte, mFontFamilyName);
#endif // UNICODE

	return mFontFamilyNameMultibyte;
}

bool CFontManager::CreateFontColor(const float r, const float g, const float b, const float a)
{
	ID2D1SolidColorBrush* brush = FindFontColor(r, g, b, a);

	if (brush)
	{
		return true;
	}

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush)))
	{
		assert(false);
		return false;
	}

	mMapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), brush));
	return true;
}

bool CFontManager::CreateFontColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	ID2D1SolidColorBrush* brush = FindFontColor(r, g, b, a);

	if (brush)
	{
		return true;
	}

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f), &brush)))
	{
		assert(false);
		return false;
	}

	mMapFontColor.insert(std::make_pair(CreateFontColorKey(r, g, b, a), brush));
	return true;
}

bool CFontManager::CreateFontColor(const Vector4& color)
{
	ID2D1SolidColorBrush* brush = FindFontColor(color);

	if (brush)
	{
		return true;
	}

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z, color.w), &brush)))
	{
		assert(false);
		return false;
	}

	mMapFontColor.insert(std::make_pair(CreateFontColorKey(color), brush));
	return true;
}

bool CFontManager::CreateFontColor(const unsigned int color)
{
	ID2D1SolidColorBrush* brush = FindFontColor(color);

	if (brush)
	{
		return true;
	}

	// argb
	// 32바이트로 저장된 unsigned int형 컬러 키에서
	// 해당 색상 값만 비트 연산해서 추출해낸 뒤, 0 ~ 1사이의 4바이트 색상으로 변환
	float r, g, b, a;
	b = (color & 0x000000ff) / 255.f;
	g = ((color >> 8) & 0x000000ff) / 255.f;
	r = ((color >> 16) & 0x000000ff) / 255.f;
	a = ((color >> 24) & 0x000000ff) / 255.f;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r,g,b,a), &brush)))
	{
		assert(false);
		return false;
	}

	mMapFontColor.insert(std::make_pair(color, brush));
	return true;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const float r, const float g, const float b, const float a)
{
	return FindFontColor(CreateFontColor(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	return FindFontColor(CreateFontColor(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const Vector4& color)
{
	return FindFontColor(CreateFontColor(color));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const unsigned int color)
{
	auto iter = mMapFontColor.find(color);

	if (iter == mMapFontColor.end())
	{
		return nullptr;
	}
	
	return iter->second;
}

unsigned int CFontManager::CreateFontColorKey(const float r, const float g, const float b, const float a)
{
	unsigned int color = 0;
	unsigned char R, G, B, A;

	R = (unsigned char)(r * 255.f);
	G = (unsigned char)(g * 255.f);
	B = (unsigned char)(b * 255.f);
	A = (unsigned char)(a * 255.f);
	
	color = A;
	color = (color << 8) | R;
	color = (color << 8) | G;
	color = (color << 8) | B;

	return color;
}

unsigned int CFontManager::CreateFontColorKey(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	unsigned int color = 0;
	
	color = a;
	color = (color << 8) | r;
	color = (color << 8) | g;
	color = (color << 8) | b;

	return color;
}

unsigned int CFontManager::CreateFontColorKey(const Vector4& color)
{
	unsigned int result = 0;
	unsigned char R, G, B, A;

	R = (unsigned char)(color.x * 255.f);
	G = (unsigned char)(color.y * 255.f);
	B = (unsigned char)(color.z * 255.f);
	A = (unsigned char)(color.w * 255.f);
	
	result = A;
	result = (result << 8) | R;
	result = (result << 8) | G;
	result = (result << 8) | B;

	return result;
}

IDWriteTextLayout* CFontManager::CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, const float width, const float height)
{
	IDWriteTextLayout* layout = nullptr;

	if (FAILED(mWriteFactory->CreateTextLayout(text, lstrlen(text), font, width, height, &layout)))
	{
		assert(false);
		return nullptr;
	}

	return layout;
}

IDWriteTextLayout* CFontManager::CreateTextLayout(const TCHAR* text, const std::string& fontName, const float width, const float height)
{
	IDWriteTextFormat* font = FindFont(fontName);

	if (!font)
	{
		assert(false);
		return nullptr;
	}

	return CreateTextLayout(text, font, width, height);
}

IDWriteFontCollection1* CFontManager::FindFontFile(const std::string& name)
{
	auto iter = mMapFontFile.find(name);

	if (iter == mMapFontFile.end())
	{
		return nullptr;
	}

	return iter->second;
}

IDWriteTextFormat* CFontManager::FindFont(const std::string& name)
{
	auto iter = mMapFont.find(name);

	if (iter == mMapFont.end())
	{
		return nullptr;
	}

	return iter->second;
}
