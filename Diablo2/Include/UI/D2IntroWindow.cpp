#include "D2IntroWindow.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Resource/Excel/ExcelData.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../Scene/D2LoadingScene.h"

CD2IntroWindow::CD2IntroWindow()
{
}

CD2IntroWindow::CD2IntroWindow(const CD2IntroWindow& window)	:
	CWidgetWindow(window)
{
}

CD2IntroWindow::~CD2IntroWindow()
{
}

bool CD2IntroWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2IntroWindow::Start()
{
	CWidgetWindow::Start();
}

void CD2IntroWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2IntroWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2IntroWindow::Render()
{
	CWidgetWindow::Render();
}

CD2IntroWindow* CD2IntroWindow::Clone()
{
	return new CD2IntroWindow(*this);
}

void CD2IntroWindow::MakeWidget(CExcelData* data)
{
	// Craete
	mBackground = CreateWidget<CImage>("IntroBack");
	mLogo = CreateWidget<CImage>("Logo");
	mStartButton = CreateWidget<CButton>("StartButton");
	mExitButton = CreateWidget<CButton>("ExitButton");
	mStartText = CreateWidget<CText>("StartText");
	mExitText = CreateWidget<CText>("ExitText");

	// Pos, Size
	std::vector<std::string>* row = data->GetRow("IntroBack");
	mBackground->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mBackground->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("Logo");
	mLogo->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mLogo->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("StartButton");
	mStartButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mStartButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("ExitButton");
	mExitButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mExitButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("StartText");
	mStartText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mStartText->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	row = data->GetRow("ExitText");
	mExitText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mExitText->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
	mBackground->SetTexture("LogoBack", TEXT("Dia2UI/gameselectscreen.png"));
	mLogo->SetTexture("Logo", TEXT("Dia2UI/logo.png"));
	for (int i = 0; i < 30; ++i)
	{
		mLogo->AddFrameData(Vector2(i * 362.f, 0.f), Vector2(362.f, 183.f));
	}
	mStartButton->SetTexture(eButtonState::Normal, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mStartButton->SetTexture(eButtonState::Hovered, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mStartButton->SetTexture(eButtonState::LClicked, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mStartButton->SetTexture(eButtonState::RClicked, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mStartButton->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(272.f, 35.f));
	mStartButton->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(272.f, 35.f));
	mStartButton->AddFrameData(eButtonState::LClicked, Vector2(272.f, 0.f), Vector2(272.f, 35.f));
	mExitButton->SetTexture(eButtonState::Normal, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mExitButton->SetTexture(eButtonState::Hovered, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mExitButton->SetTexture(eButtonState::LClicked, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mExitButton->SetTexture(eButtonState::RClicked, "HalfWideButton", TEXT("Dia2UI/HalfWideButton.png"));
	mExitButton->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(272.f, 35.f));
	mExitButton->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(272.f, 35.f));
	mExitButton->AddFrameData(eButtonState::LClicked, Vector2(272.f, 0.f), Vector2(272.f, 35.f));

	mStartText->SetFont("Dia2Font");
	mStartText->SetAlignH(eTextAlignH::Center);
	mStartText->SetAlignV(eTextAlignV::Middle);
	mStartText->SetColor(1.f, 1.f, 1.f);
	mStartText->SetText(TEXT("Start"));
	mStartText->SetShadowEnable(true);
	mStartText->SetShadowColor(0.f, 0.f, 0.f);
	mStartText->SetShadowOffset(1.f, 1.f);
	mExitText->SetFont("Dia2Font");
	mExitText->SetAlignH(eTextAlignH::Center);
	mExitText->SetAlignV(eTextAlignV::Middle);
	mExitText->SetColor(1.f, 1.f, 1.f);
	mExitText->SetText(TEXT("Exit"));
	mExitText->SetShadowEnable(true);
	mExitText->SetShadowColor(0.f, 0.f, 0.f);
	mExitText->SetShadowOffset(1.f, 1.f);

	// CallBack
	mStartButton->SetLClickCallBack(this, &CD2IntroWindow::onClickStart);
	mExitButton->SetLClickCallBack(this, &CD2IntroWindow::onClickEnd);
}

void CD2IntroWindow::onClickStart()
{
	CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CD2LoadingScene>(false);
}

void CD2IntroWindow::onClickEnd()
{
	CEngine::GetInst()->Exit();
}
