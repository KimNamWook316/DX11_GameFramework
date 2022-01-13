#include "Text.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"
#include "../Device.h"

CText::CText()	:
	mTextCount(0),
	mTextCapacity(32),
	mFont(nullptr),
	mLayout(nullptr),
	mColorBrush(nullptr),
	m2DTarget(nullptr),
	mFontSize(20.f),
	mbAlphaEnable(true),
	mOpacity(1.0f),
	mbShadowEnable(false),
	mShadowColorBrush(nullptr),
	mbShadowAlphaEnable(true),
	mShadowOpacity(1.0f),
	mShadowOffset(1.f, 1.f),
	meAlignH(eTextAlignH::Left),
	meAlignV(eTextAlignV::Middle)
{
	mText = new TCHAR[mTextCapacity];

	memset(mText, 0, sizeof(TCHAR) * mTextCapacity);

	mFontName = new TCHAR[128];

	memset(mFontName, 0, sizeof(TCHAR) * 128);

	lstrcpy(mText, TEXT("Text"));

	mTextCount = 4;
}

CText::CText(const CText& widget)	:
	CWidget(widget)
{
	*this = widget;

	mText = new TCHAR[mTextCapacity];
	memset(mText, 0, sizeof(TCHAR) * mTextCapacity);

	if (mTextCount > 0)
	{
		memcpy(mText, widget.mText, sizeof(TCHAR) * mTextCount);
	}

	mFontName = new TCHAR[128];
	memset(mFontName, 0, sizeof(TCHAR) * 128);
	lstrcpy(mFontName, widget.mFontName);

	mLayout = nullptr;
	CreateTextLayout();
}

CText::~CText()
{
	SAFE_DELETE(mLayout);
	SAFE_DELETE_ARRAY(mText);
	SAFE_DELETE_ARRAY(mFontName);
}

bool CText::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}

	m2DTarget = CDevice::GetInst()->Get2DRenderTarget();
	const TCHAR* fontName = CResourceManager::GetInst()->GetFontFaceName("Default");
	lstrcpy(mFontName, fontName);

	mFontKey = "Default";
	mFont = CResourceManager::GetInst()->FindFont(mFontKey);

	CResourceManager::GetInst()->CreateFontColor(mColor);
	mColorBrush = CResourceManager::GetInst()->FindFontColor(mColor);

	CResourceManager::GetInst()->CreateFontColor(mShadowColor);
	mShadowColorBrush = CResourceManager::GetInst()->FindFontColor(mShadowColor);

	CreateTextLayout();
	return true;
}

void CText::Start()
{
	CWidget::Start();
}

void CText::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CText::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CText::Render()
{
	m2DTarget->BeginDraw();

	// Window 좌표계를 사용하여 그리므로, 월드 좌표에서 변환해준다.
	Resolution RS = CDevice::GetInst()->GetResolution();
	D2D1_POINT_2F point;
	point.x = mRenderPos.x;
	point.y = RS.Height - mRenderPos.y - mSize.y;

	// 그림자를 먼저 그린다.
	if (mbShadowEnable)
	{
		D2D1_POINT_2F shadowPoint = point;
		shadowPoint.x += mShadowOffset.x;
		shadowPoint.y += mShadowOffset.y;
		
		if (mbShadowAlphaEnable)
		{
			mShadowColorBrush->SetOpacity(mShadowOpacity);
		}
		else
		{
			mShadowColorBrush->SetOpacity(1.0f);
		}

		m2DTarget->DrawTextLayout(shadowPoint, mLayout, mShadowColorBrush,
			D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	// 텍스트를 그린다.
	if (mbAlphaEnable)
	{
		mColorBrush->SetOpacity(mShadowOpacity);
	}
	else
	{
		mColorBrush->SetOpacity(1.0f);
	}
	
	m2DTarget->DrawTextLayout(point, mLayout, mColorBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE);
	
	m2DTarget->EndDraw();
}

bool CText::CreateTextLayout()
{
	// 폰트가 없으면 레이아웃 만들지 않는다.
	if (!mFont)
	{
		assert(false);
		return false;
	}

	// 현재 있는 레이아웃 파기
	SAFE_RELEASE(mLayout);

	// 레이아웃 생성 
	mLayout = CResourceManager::GetInst()->CreateTextLayout(mText, mFont, mSize.x, mSize.y);

	DWRITE_TEXT_RANGE range = {};
	range.startPosition = 0;
	range.length = lstrlen(mText);

	mLayout->SetFontSize(mFontSize, range);

	switch (meAlignH)
	{
	case eTextAlignH::Left:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case eTextAlignH::Center:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case eTextAlignH::Right:
		mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	default:
		assert(false);
		return false;
	}

	switch (meAlignV)
	{
	case eTextAlignV::Top:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case eTextAlignV::Middle:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case eTextAlignV::Bottom:
		mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	default:
		assert(false);
		return false;
	}

	return true;
}

void CText::SetFont(const std::string& name)
{
	mFontKey = name;
	mFont = CResourceManager::GetInst()->FindFont(name);

	CreateTextLayout();
}

void CText::SetFontSize(const float size)
{
	mFontSize = size;
	CreateTextLayout();
}

void CText::SetAlignH(eTextAlignH align)
{
	meAlignH = align;
	CreateTextLayout();
}

void CText::SetAlignV(eTextAlignV align)
{
	meAlignV = align;
	CreateTextLayout();
}

void CText::SetColor(const float r, const float g, const float b)
{
	mColor.x = r;
	mColor.y = g;
	mColor.z = b;
	
	CResourceManager::GetInst()->CreateFontColor(mColor);
	mColorBrush = CResourceManager::GetInst()->FindFontColor(mColor);
}

void CText::SetColor(const unsigned char r, const unsigned char g, const unsigned char b)
{
	mColor.x = r / 255.f;
	mColor.y = g / 255.f;
	mColor.z = b / 255.f;
	
	CResourceManager::GetInst()->CreateFontColor(mColor);
	mColorBrush = CResourceManager::GetInst()->FindFontColor(mColor);
}

void CText::SetColor(const Vector4& color)
{
	mColor.x = color.x;
	mColor.y = color.y;
	mColor.z = color.z;
	
	CResourceManager::GetInst()->CreateFontColor(mColor);
	mColorBrush = CResourceManager::GetInst()->FindFontColor(mColor);
}

void CText::SetColor(unsigned int color)
{
	mColor.x = ((color >> 16) & 0x000000ff) / 255.f;
	mColor.y = ((color >> 8) & 0x000000ff) / 255.f;
	mColor.z = (color & 0x000000ff) / 255.f;
	
	CResourceManager::GetInst()->CreateFontColor(mColor);
	mColorBrush = CResourceManager::GetInst()->FindFontColor(mColor);
}

void CText::SetAlphaEnable(bool bAlpha)
{
	mbAlphaEnable = bAlpha;
}

void CText::SetOpacity(const float opacity)
{
	mOpacity = opacity;
}

void CText::SetShadowEnable(bool bShadow)
{
	mbShadowEnable = bShadow;
}

void CText::SetShadowOffset(const Vector2& offset)
{
	mShadowOffset = offset;
}

void CText::SetShadowOffset(const float x, const float y)
{
	mShadowOffset.x = x;
	mShadowOffset.y = y;
}

void CText::SetShadowColor(const float r, const float g, const float b)
{
	mShadowColor.x = r;
	mShadowColor.y = g;
	mShadowColor.z = b;
	
	CResourceManager::GetInst()->CreateFontColor(mShadowColor);
	mShadowColorBrush = CResourceManager::GetInst()->FindFontColor(mShadowColor);
}

void CText::SetShadowColor(const unsigned char r, const unsigned char g, const unsigned char b)
{
	mShadowColor.x = r;
	mShadowColor.y = g;
	mShadowColor.z = b;
	
	CResourceManager::GetInst()->CreateFontColor(mShadowColor);
	mShadowColorBrush = CResourceManager::GetInst()->FindFontColor(mShadowColor);
}

void CText::SetShadowColor(const Vector4& color)
{
	mShadowColor.x = color.x;
	mShadowColor.y = color.y;
	mShadowColor.z = color.z;
	
	CResourceManager::GetInst()->CreateFontColor(mShadowColor);
	mShadowColorBrush = CResourceManager::GetInst()->FindFontColor(mShadowColor);
}

void CText::SetShadowColor(unsigned int color)
{
	mShadowColor.x = ((color >> 16) & 0x000000ff) / 255.f;
	mShadowColor.y = ((color >> 8) & 0x000000ff) / 255.f;
	mShadowColor.z = (color & 0x000000ff) / 255.f;
	
	CResourceManager::GetInst()->CreateFontColor(mShadowColor);
	mShadowColorBrush = CResourceManager::GetInst()->FindFontColor(mShadowColor);
}

void CText::SetShadowAlphaEnable(bool bAlpha)
{
	mbShadowAlphaEnable = bAlpha;
}

void CText::SetShadowOpacity(const float opacity)
{
	mShadowOpacity = opacity;
}
