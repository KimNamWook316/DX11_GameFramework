#include "MainWidget.h"
#include "Engine.h"

CMainWidget::CMainWidget()
{
}

CMainWidget::CMainWidget(const CMainWidget& window)
{
	mButton = FindWidget<CButton>("Button");
	mText = FindWidget<CText>("Text");
	mHour = FindWidget<CNumber>("Hour");
	mMinute = FindWidget<CNumber>("Minute");
	mSecond = FindWidget<CNumber>("Second");
	mFPSText = FindWidget<CText>("FPSText");
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
	if (!CWidgetWindow::Init())
	{
		return false;
	}
	
	SetSize(1280.f, 720.f);

	mButton = CreateWidget<CButton>("Button");

	mButton->SetPos(400.f, 300.f);
	mButton->SetSize(200.f, 50.f);
	mButton->SetTexture(eButtonState::Normal, "StartButton", TEXT("Start.png"));
	mButton->SetTexture(eButtonState::Hovered, "StartButton", TEXT("Start.png"));
	mButton->SetTexture(eButtonState::LClicked, "StartButton", TEXT("Start.png"));

	mButton->SetTextureTint(eButtonState::Normal, 220, 220, 220, 255);
	mButton->SetTextureTint(eButtonState::Hovered, 255, 255, 255, 255);
	mButton->SetTextureTint(eButtonState::LClicked, 128, 128, 128, 128);

	mButton->SetLClickCallBack(this, &CMainWidget::onStartButtonClicked);
	
	mButton->SetSound(eButtonSoundState::Hovered, "UI", "ButtonMouseOn", "TeemoSmile.mp3");
	mButton->SetSound(eButtonSoundState::Clicked, "UI", "ButtonClick", "TeemoStartClicck.mp3");

	mText = CreateWidget<CText>("Text");
	mText->SetText(TEXT("Hello"));
	mText->SetPos(200.f, 400.f);
	mText->SetSize(100.f, 40.f);
	mText->SetZorder(1);
	mText->SetAlignH(eTextAlignH::Center);
	mText->SetAlignV(eTextAlignV::Middle);
	mText->SetShadowEnable(true);
	mText->SetShadowColor(0.f, 1.f, 0.f);
	mText->SetShadowOffset(2.f, 2.f);

	mHour = CreateWidget<CNumber>("Hour");
	mMinute = CreateWidget<CNumber>("Minute");
	mSecond = CreateWidget<CNumber>("Second");

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(fileName);
	}

	mHour->SetTexture("Number", vecFileName);
	mHour->AddFrameDataFrameTexture(10);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}
	vecFileName.clear();

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(fileName);
	}

	mMinute->SetTexture("Number", vecFileName);
	mMinute->AddFrameDataFrameTexture(10);
	mMinute->SetPos(100.f, 0.f);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}
	vecFileName.clear();

	for (int i = 0; i < 10; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(fileName);
	}

	mSecond->SetTexture("Number", vecFileName);
	mSecond->AddFrameDataFrameTexture(10);
	mSecond->SetPos(200.f, 0.f);

	for (int i = 0; i < 10; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}
	vecFileName.clear();

	mFPSText = CreateWidget<CText>("FPSText");
	mFPSText->SetText(TEXT("FPSText"));
	mFPSText->SetPos(900.f, 650.f);
	mFPSText->SetSize(300.f, 40.f);
	mFPSText->SetZorder(1);
	mFPSText->SetColor(0.f, 0.f, 1.f);
	mFPSText->SetAlignH(eTextAlignH::Center);
	mFPSText->SetShadowEnable(true);
	mFPSText->SetShadowOffset(2.f, 2.f);

	return true;
}

void CMainWidget::Start()
{
	CWidgetWindow::Start();
}

void CMainWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);

	SYSTEMTIME time;

	GetLocalTime(&time);

	mHour->SetNumber((int)time.wHour);
	mMinute->SetNumber((int)time.wMinute);
	mSecond->SetNumber((int)time.wSecond);

	char FPS[256] = {};
	sprintf_s(FPS, "FPS : %.5f", CEngine::GetInst()->GetFPS());

	TCHAR convertFPS[256] = {};

	int length = MultiByteToWideChar(CP_ACP, 0, FPS, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FPS, -1, convertFPS, length);

	mFPSText->SetText(convertFPS);
}

void CMainWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMainWidget::Render()
{
	CWidgetWindow::Render();
}

CMainWidget* CMainWidget::Clone()
{
	return new CMainWidget(*this);
}

void CMainWidget::onStartButtonClicked()
{
	//MessageBox(nullptr, TEXT("Click"), TEXT("Click"), MB_OK);
}
