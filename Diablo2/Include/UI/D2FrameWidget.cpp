#include "D2FrameWidget.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "../Component/D2PlayerSkillComponent.h"
#include "../Component/D2CharacterInfoComponent.h"

CD2FrameWidget::CD2FrameWidget()
{
}

CD2FrameWidget::CD2FrameWidget(const CD2FrameWidget& window)
{
	mFPSText = FindWidget<CText>("FPSText");
	mPlayerSkill = FindWidget<CText>("PlayerSkill");
}

CD2FrameWidget::~CD2FrameWidget()
{
}

bool CD2FrameWidget::Init()
{
	if (!CWidgetWindow::Init())
	{
		return false;
	}
	
	SetSize(800.f, 600.f);

	mFPSText = CreateWidget<CText>("FPSText");
	mFPSText->SetText(TEXT("FPSText"));
	mFPSText->SetPos(600.f, 500.f);
	mFPSText->SetSize(300.f, 40.f);
	mFPSText->SetZorder(1);
	mFPSText->SetColor(1.f, 1.f, 1.f);
	mFPSText->SetAlignH(eTextAlignH::Center);
	mFPSText->SetShadowEnable(true);
	mFPSText->SetShadowOffset(2.f, 2.f);

	mPlayerSkill = CreateWidget<CText>("PlayerSkill");
	mPlayerSkill->SetText(TEXT("PlayerSkill"));
	mPlayerSkill->SetPos(0.f, 500.f);
	mPlayerSkill->SetSize(300.f, 40.f);
	mPlayerSkill->SetZorder(1);
	mPlayerSkill->SetColor(1.f, 1.f, 1.f);
	mPlayerSkill->SetAlignH(eTextAlignH::Center);



	return true;
}

void CD2FrameWidget::Start()
{
	CWidgetWindow::Start();
}

void CD2FrameWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);

	char FPS[256] = {};
	sprintf_s(FPS, "FPS : %.5f", CEngine::GetInst()->GetFPS());

	TCHAR convertFPS[256] = {};

	int length = MultiByteToWideChar(CP_ACP, 0, FPS, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FPS, -1, convertFPS, length);

	mFPSText->SetText(convertFPS);

 //	std::string skillName = mSkill->GetCurrentSkillName();
 //	length = MultiByteToWideChar(CP_ACP, 0, skillName.c_str(), -1, 0, 0);
 //	MultiByteToWideChar(CP_ACP, 0, skillName.c_str(), -1, convertFPS, length);
 //
 //
 //	mPlayerSkill->SetText(convertFPS);
 }

void CD2FrameWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2FrameWidget::Render()
{
	CWidgetWindow::Render();
}

CD2FrameWidget* CD2FrameWidget::Clone()
{
	return new CD2FrameWidget(*this);
}

void CD2FrameWidget::OnClickGrid(int idx)
{
	char index[128] = {};
	sprintf_s(index, "Index : %d", idx);
	TCHAR convertIndex[128] = {};
	int length = MultiByteToWideChar(CP_ACP, 0, index, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, index, -1, convertIndex, length);
	MessageBox(nullptr, convertIndex, convertIndex, MB_OK);
}
