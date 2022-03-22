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
	
	SetSize(1280.f, 720.f);

	mFPSText = CreateWidget<CText>("FPSText");
	mFPSText->SetText(TEXT("FPSText"));
	mFPSText->SetPos(500.f, 500.f);
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

	mHp = CreateWidget<CText>("Hp");
	mHp->SetText(TEXT("Hp"));
	mHp->SetPos(0.f, 450.f);
	mHp->SetSize(300.f, 40.f);
	mHp->SetZorder(1);
	mHp->SetColor(1.f, 1.f, 1.f);
	mHp->SetAlignH(eTextAlignH::Center);

	mMp = CreateWidget<CText>("Mp");
	mMp->SetText(TEXT("Mp"));
	mMp->SetPos(0.f, 400.f);
	mMp->SetSize(300.f, 40.f);
	mMp->SetZorder(1);
	mMp->SetColor(1.f, 1.f, 1.f);
	mMp->SetAlignH(eTextAlignH::Center);

 //	mImage = CreateWidget<CImage>("Image");
 //	mImage->SetTexture("UIBorderRight", TEXT("Dia2UI/800BorderRight.png"));
 //	mImage->SetPos(400.f, 0.f);
 //	mImage->SetSize(400.f, 600.f);
 //
 //	mImage2 = CreateWidget<CImage>("Image2");
 //	mImage2->SetTexture("ItemPanel", TEXT("Dia2UI/inven_panel.png"));
 //	mImage2->SetPos(400.f, 104.f);
 //	mImage2->SetSize(320.f, 432.f);

	mGrid = CreateWidget<CGrid>("Grid");
	mGrid->SetPos(500.f, 100.f);
	mGrid->SetCellCount(10, 10);
	mGrid->SetCellSize(30.f, 30.f);
	mGrid->SetTint(eCellState::Normal, Vector4(1.f, 1.f, 1.f, 0.7f));
	mGrid->SetTint(eCellState::Hovered, Vector4(0.f, 1.f, 0.f, 0.7f));
	mGrid->SetTint(eCellState::Clicked, Vector4(0.f, 0.f, 1.f, 0.7f));
	mGrid->OccupyCell(99);
	mGrid->SetSelectRange(2, 3);
	mGrid->SetCallBack(eCellState::Clicked, this, &CD2FrameWidget::OnClickGrid);

	mSkill = mViewport->GetScene()->FindObject("Player")->FindObjectComponentFromType<CD2PlayerSkillComponent>();
	mCharInfo = mViewport->GetScene()->FindObject("Player")->FindObjectComponentFromType<CD2CharacterInfoComponent>();

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

	std::string skillName = mSkill->GetCurrentSkillName();
	length = MultiByteToWideChar(CP_ACP, 0, skillName.c_str(), -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, skillName.c_str(), -1, convertFPS, length);

	char Hp[64] = {};
	TCHAR convertHp[64] = {};
	sprintf_s(Hp, "Hp : %.5f", mCharInfo->GetHp());
	length = MultiByteToWideChar(CP_ACP, 0, Hp, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, Hp, -1, convertHp, length);

	mHp->SetText(convertHp);

	sprintf_s(Hp, "Mp : %.5f", mCharInfo->GetMp());
	length = MultiByteToWideChar(CP_ACP, 0, Hp, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, Hp, -1, convertHp, length);

	mMp->SetText(convertHp);

	mPlayerSkill->SetText(convertFPS);
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
