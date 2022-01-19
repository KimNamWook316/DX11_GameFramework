#include "MouseNormal.h"

CMouseNormal::CMouseNormal()
{
	SetTypeID<CMouseNormal>();
}

CMouseNormal::CMouseNormal(const CMouseNormal& window)	:
	CWidgetWindow(window)
{
	mImage = FindWidget<CImage>("Image");
}

CMouseNormal::~CMouseNormal()
{
}

bool CMouseNormal::Init()
{
	if (!CWidgetWindow::Init())
	{
		assert(false);
		return false;
	}
	
	SetSize(32.f, 31.f);

	mImage = CreateWidget<CImage>("Image");
	
	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i <= 12; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Mouse/Default/%d.png"), i);
		vecFileName.push_back(fileName);
	}

	mImage->SetTexture("MouseNormal", vecFileName);
	mImage->SetSize(32.f, 31.f);
	mImage->AddFrameDataFrameTexture(13);

	for (int i = 0; i <= 12; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	return true;
}

void CMouseNormal::Start()
{
	CWidgetWindow::Start();
}

void CMouseNormal::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CMouseNormal::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMouseNormal::Render()
{
	CWidgetWindow::Render();
}

CMouseNormal* CMouseNormal::Clone()
{
	return new CMouseNormal(*this);
}
