#include "MouseAttack.h"

CMouseAttack::CMouseAttack()
{
	SetTypeID<CMouseAttack>();
}

CMouseAttack::CMouseAttack(const CMouseAttack& window)	:
	CWidgetWindow(window)
{
	mImage = FindWidget<CImage>("Image");
}

CMouseAttack::~CMouseAttack()
{
}

bool CMouseAttack::Init()
{
	if (!CWidgetWindow::Init())
	{
		assert(false);
		return false;
	}
	
	SetSize(32.f, 31.f);

	mImage = CreateWidget<CImage>("Image");
	
	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i <= 6; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Mouse/Temp/Attack/000%d.png"), i);
		vecFileName.push_back(fileName);
	}

	mImage->SetTexture("MouseAttack", vecFileName);
	mImage->SetSize(30.f, 30.f);
	mImage->AddFrameDataFrameTexture(7);

	for (int i = 0; i <= 6; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	return true;
}

void CMouseAttack::Start()
{
	CWidgetWindow::Start();
}

void CMouseAttack::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CMouseAttack::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMouseAttack::Render()
{
	CWidgetWindow::Render();
}

CMouseAttack* CMouseAttack::Clone()
{
	return new CMouseAttack(*this);
}
