#include "StartWidget.h"
#include "Engine.h"
#include "../Scene/LoadingScene.h"
#include "Scene/SceneManager.h"

CStartWidget::CStartWidget()
{
}

CStartWidget::CStartWidget(const CStartWidget& window)	:
	CWidgetWindow(window)
{
}

CStartWidget::~CStartWidget()
{
}

bool CStartWidget::Init()
{
	if (!CWidgetWindow::Init())
	{
		assert(false);
		return false;
	}

	SetSize(1280.f, 720.f);

	mStartButton = CreateWidget<CButton>("StartButton");
	mExitButton = CreateWidget<CButton>("ExitButton");
	mBack = CreateWidget<CImage>("Back");

	mStartButton->SetZorder(1);
	mExitButton->SetZorder(1);

	mBack->SetTexture("StartBack", TEXT("Teemo.jpg"));
	mBack->SetSize(1280.f, 720.f);

	mStartButton->SetPos(1280.f / 2.f - 100.f, 720.f / 2.f + 50.f);
	mStartButton->SetSize(200.f, 50.f);
	mStartButton->SetTexture(eButtonState::Normal, "StartButton", TEXT("Start.png"));
	mStartButton->SetTexture(eButtonState::Hovered, "StartButton", TEXT("Start.png"));
	mStartButton->SetTexture(eButtonState::Clicked, "StartButton", TEXT("Start.png"));
	mStartButton->SetTextureTint(eButtonState::Normal, 220, 220, 220, 255);
	mStartButton->SetTextureTint(eButtonState::Hovered, 255, 255, 255, 255);
	mStartButton->SetTextureTint(eButtonState::Clicked, 150, 150, 150, 255);
	mStartButton->SetSound(eButtonSoundState::Hovered, "UI", "ButtonHovered", "TeemoSmile.mp3");
	mStartButton->SetSound(eButtonSoundState::Hovered, "UI", "ButtonClick", "TeemoStartClicck.mp3");
	mStartButton->SetClickCallBack(this, &CStartWidget::OnClickStart);

	mExitButton->SetPos(1280.f / 2.f - 100.f, 720.f / 2.f - 100.f);
	mExitButton->SetSize(200.f, 50.f);
	mExitButton->SetTexture(eButtonState::Normal, "EndButton", TEXT("End.png"));
	mExitButton->SetTexture(eButtonState::Hovered, "EndButton", TEXT("End.png"));
	mExitButton->SetTexture(eButtonState::Clicked, "EndButton", TEXT("End.png"));
	mExitButton->SetTextureTint(eButtonState::Normal, 220, 220, 220, 255);
	mExitButton->SetTextureTint(eButtonState::Hovered, 255, 255, 255, 255);
	mExitButton->SetTextureTint(eButtonState::Clicked, 150, 150, 150, 255);
	mExitButton->SetSound(eButtonSoundState::Hovered, "UI", "ButtonHovered", "TeemoSmile.mp3");
	mExitButton->SetSound(eButtonSoundState::Hovered, "UI", "ButtonClick", "TeemoStartClicck.mp3");
	mExitButton->SetClickCallBack(this, &CStartWidget::OnClickExit);

	return true;
}

void CStartWidget::Start()
{
	CWidgetWindow::Start();
}

void CStartWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CStartWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CStartWidget::Render()
{
	CWidgetWindow::Render();
}

CStartWidget* CStartWidget::Clone()
{
	return new CStartWidget(*this);
}

void CStartWidget::OnClickStart()
{
	CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CLoadingScene>(false);
}

void CStartWidget::OnClickExit()
{
	CEngine::GetInst()->Exit();
}
