#include "D2SkillTreeWindow.h"
#include "../Component/D2PlayerSkillComponent.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "Resource/Excel/ExcelData.h"
#include "Scene/Viewport.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "../Component/D2UIManager.h"
#include "D2StatusBarWindow.h"

CD2SkillTreeWindow::CD2SkillTreeWindow()	:
	mSkill(nullptr)
{
}

CD2SkillTreeWindow::CD2SkillTreeWindow(const CD2SkillTreeWindow& window)	:
	CWidgetWindow(window)
{
}

CD2SkillTreeWindow::~CD2SkillTreeWindow()
{
}

bool CD2SkillTreeWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2SkillTreeWindow::Start()
{
	CWidgetWindow::Start();

	mSkill = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2PlayerSkillComponent>();
	mCharInfo = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	onChangeSkillPoint();
	onInit();

	mSkill->SetStateChangeCallBackAll(this, &CD2SkillTreeWindow::onStateChange);
	mCharInfo->SetEventCallBack("OnLevelUp", this, &CD2SkillTreeWindow::onChangeSkillPoint, eD2CharInfoEventType::LevelUp);
}

void CD2SkillTreeWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2SkillTreeWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2SkillTreeWindow::Render()
{
	CWidgetWindow::Render();
}

CD2SkillTreeWindow* CD2SkillTreeWindow::Clone()
{
	return new CD2SkillTreeWindow(*this);
}

void CD2SkillTreeWindow::MakeWidget(CExcelData* data)
{
	// Create Widget
	mSkillPointPanel = CreateWidget<CImage>("SkillPointPanel");
	mSkillPointLabelText = CreateWidget<CText>("SkillPointLabel");
	mSkillPointText = CreateWidget<CText>("SkillPointText");
	mSkillTree1Panel = CreateWidget<CImage>("SkillTree1Panel");
	mSkillTree2Panel = CreateWidget<CImage>("SkillTree2Panel");
	mSkillTree3Panel = CreateWidget<CImage>("SkillTree3Panel");
	mSkillTree1Button = CreateWidget<CButton>("SkillTree1Button");
	mSkillTree2Button = CreateWidget<CButton>("SkillTree2Button");
	mSkillTree3Button = CreateWidget<CButton>("SkillTree3Button");
	mSkillTree1Text = CreateWidget<CText>("SkillTree1Text");
	mSkillTree2Text = CreateWidget<CText>("SkillTree2Text");
	mSkillTree3Text = CreateWidget<CText>("SkillTree3Text");
	mSkillTree1ExitButton = CreateWidget<CButton>("SkillTree1ExitButton");
	mSkillTree2ExitButton = CreateWidget<CButton>("SkillTree2ExitButton");
	mSkillTree3ExitButton = CreateWidget<CButton>("SkillTree3ExitButton");
	mFireBoltButton.Button = CreateWidget<CButton>("FireBoltButtonButton");
	mFireBoltButton.LevelText = CreateWidget<CText>("FireBoltButtonText");
	mFireBallButton.Button = CreateWidget<CButton>("FireBallButtonButton");
	mFireBallButton.LevelText = CreateWidget<CText>("FireBallButtonText");
	mBlazeButton.Button = CreateWidget<CButton>("BlazeButtonButton");
	mBlazeButton.LevelText = CreateWidget<CText>("BlazeButtonText");
	mMeteorButton.Button = CreateWidget<CButton>("MeteorButtonButton");
	mMeteorButton.LevelText = CreateWidget<CText>("MeteorButtonText");
	mIceBoltButton.Button = CreateWidget<CButton>("IceBoltButtonButton");
	mIceBoltButton.LevelText = CreateWidget<CText>("IceBoltButtonText");
	mFrozenOrbButton.Button = CreateWidget<CButton>("FrozenOrbButtonButton");
	mFrozenOrbButton.LevelText = CreateWidget<CText>("FrozenOrbButtonText");
	mTeleportButton.Button = CreateWidget<CButton>("TeleportButtonButton");
	mTeleportButton.LevelText = CreateWidget<CText>("TeleportButtonText");

	// Position, Size
	std::vector<std::string>* row = data->GetRow("SkillPoint");
	mSkillPointPanel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillPointPanel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPointLabelText");
	mSkillPointLabelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillPointLabelText->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPointText");
	mSkillPointText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillPointText->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBottom");
	mSkillTree1Panel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree1Panel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelMid");
	mSkillTree2Panel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree2Panel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBottom");
	mSkillTree3Panel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree3Panel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBottom");
	mSkillTree3Panel->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree3Panel->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBottomButton");
	mSkillTree1Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree1Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelMidButton");
	mSkillTree2Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree2Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelTopButton");
	mSkillTree3Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree3Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBottomText");
	mSkillTree1Text->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree1Text->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelMidText");
	mSkillTree2Text->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree2Text->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelTopText");
	mSkillTree3Text->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree3Text->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBottomExit");
	mSkillTree1ExitButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree1ExitButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelMidExit");
	mSkillTree2ExitButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree2ExitButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelTopExit");
	mSkillTree3ExitButton->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mSkillTree3ExitButton->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelFireBolt");
	mFireBoltButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mFireBoltButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelFireBall");
	mFireBallButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mFireBallButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelBlaze");
	mBlazeButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mBlazeButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelMeteor");
	mMeteorButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mMeteorButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelIceBolt");
	mIceBoltButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mIceBoltButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelFrozenOrb");
	mFrozenOrbButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mFrozenOrbButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelTeleport");
	mTeleportButton.Button->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mTeleportButton.Button->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("SkillPanelLevelOffset");
	Vector2 buttonPos = mFireBoltButton.Button->GetWidgetPos();
	mFireBoltButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mFireBoltButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]),buttonPos.y - std::stof((*row)[4]));
	buttonPos = mFireBallButton.Button->GetWidgetPos();
	mFireBallButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mFireBallButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]), buttonPos.y - std::stof((*row)[4]));
	buttonPos = mBlazeButton.Button->GetWidgetPos();
	mBlazeButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mBlazeButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]), buttonPos.y - std::stof((*row)[4]));
	buttonPos = mMeteorButton.Button->GetWidgetPos();
	mMeteorButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mMeteorButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]), buttonPos.y - std::stof((*row)[4]));
	buttonPos = mIceBoltButton.Button->GetWidgetPos();
	mIceBoltButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mIceBoltButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]),buttonPos.y - std::stof((*row)[4]));
	buttonPos = mFrozenOrbButton.Button->GetWidgetPos();
	mFrozenOrbButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mFrozenOrbButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]),buttonPos.y - std::stof((*row)[4]));
	buttonPos = mTeleportButton.Button->GetWidgetPos();
	mTeleportButton.LevelText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mTeleportButton.LevelText->SetPos(buttonPos.x + std::stof((*row)[3]), buttonPos.y - std::stof((*row)[4]));

	// Init
	mSkillPointPanel->SetTexture("SkillPointPanel", TEXT("Dia2UI/SkillPoint.png"));
	mSkillTree1Panel->SetTexture("SkillTree1Panel", TEXT("Dia2UI/SkillTreeBottom.png"));
	mSkillTree2Panel->SetTexture("SkillTree2Panel", TEXT("Dia2UI/SkillTreeMid.png"));
	mSkillTree3Panel->SetTexture("SkillTree3Panel", TEXT("Dia2UI/SkillTreeTop.png"));

	mSkillTree1Button->SetTextureTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree2Button->SetTextureTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree3Button->SetTextureTint(eButtonState::Normal, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree1Button->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree2Button->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree3Button->SetTextureTint(eButtonState::Hovered, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree1Button->SetTextureTint(eButtonState::LClicked, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree2Button->SetTextureTint(eButtonState::LClicked, Vector4(0.f, 0.f, 0.f, 0.f));
	mSkillTree3Button->SetTextureTint(eButtonState::LClicked, Vector4(0.f, 0.f, 0.f, 0.f));

	mSkillTree1ExitButton->SetTexture(eButtonState::Normal, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree1ExitButton->SetTexture(eButtonState::Hovered, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree1ExitButton->SetTexture(eButtonState::LClicked, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree1ExitButton->AddFrameData(eButtonState::Normal, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree1ExitButton->AddFrameData(eButtonState::Hovered, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree1ExitButton->AddFrameData(eButtonState::LClicked, Vector2(352.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree2ExitButton->SetTexture(eButtonState::Normal, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree2ExitButton->SetTexture(eButtonState::Hovered, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree2ExitButton->SetTexture(eButtonState::LClicked, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree2ExitButton->AddFrameData(eButtonState::Normal, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree2ExitButton->AddFrameData(eButtonState::Hovered, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree2ExitButton->AddFrameData(eButtonState::LClicked, Vector2(352.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree3ExitButton->SetTexture(eButtonState::Normal, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree3ExitButton->SetTexture(eButtonState::Hovered, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree3ExitButton->SetTexture(eButtonState::LClicked, "Buttons", TEXT("Dia2UI/buysell_button.png"));
	mSkillTree3ExitButton->AddFrameData(eButtonState::Normal, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree3ExitButton->AddFrameData(eButtonState::Hovered, Vector2(320.f, 0.f), Vector2(32.f, 32.f));
	mSkillTree3ExitButton->AddFrameData(eButtonState::LClicked, Vector2(352.f, 0.f), Vector2(32.f, 32.f));

	mFireBoltButton.Button->SetTexture(eButtonState::Normal, "FireBoltIcon", TEXT("Dia2UI/SkillIcon/IconFireBolt.png"));
	mFireBoltButton.Button->SetTexture(eButtonState::Hovered, "FireBoltIcon", TEXT("Dia2UI/SkillIcon/IconFireBolt.png"));
	mFireBoltButton.Button->SetTexture(eButtonState::Disabled, "FireBoltIcon", TEXT("Dia2UI/SkillIcon/IconFireBolt.png"));
	mFireBoltButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFireBoltButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFireBoltButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFireBoltButton.Button->SetTexture(eButtonState::LClicked, "FireBoltIcon", TEXT("Dia2UI/SkillIcon/IconFireBolt.png"));
	mFireBoltButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mFireBoltButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mFireBoltButton.Button->ButtonEnable(false);

	mFireBallButton.Button->SetTexture(eButtonState::Normal, "FireBallIcon", TEXT("Dia2UI/SkillIcon/IconFireBall.png"));
	mFireBallButton.Button->SetTexture(eButtonState::Hovered, "FireBallIcon", TEXT("Dia2UI/SkillIcon/IconFireBall.png"));
	mFireBallButton.Button->SetTexture(eButtonState::Disabled, "FireBallIcon", TEXT("Dia2UI/SkillIcon/IconFireBall.png"));
	mFireBallButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFireBallButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFireBallButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFireBallButton.Button->SetTexture(eButtonState::LClicked, "FireBallIcon", TEXT("Dia2UI/SkillIcon/IconFireBall.png"));
	mFireBallButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mFireBallButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mFireBallButton.Button->ButtonEnable(false);

	mBlazeButton.Button->SetTexture(eButtonState::Normal, "BlazeButton", TEXT("Dia2UI/SkillIcon/IconBlaze.png"));
	mBlazeButton.Button->SetTexture(eButtonState::Hovered, "BlazeButton", TEXT("Dia2UI/SkillIcon/IconBlaze.png"));
	mBlazeButton.Button->SetTexture(eButtonState::Disabled, "BlazeButton", TEXT("Dia2UI/SkillIcon/IconBlaze.png"));
	mBlazeButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mBlazeButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mBlazeButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mBlazeButton.Button->SetTexture(eButtonState::LClicked, "BlazeButton", TEXT("Dia2UI/SkillIcon/IconBlaze.png"));
	mBlazeButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mBlazeButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mBlazeButton.Button->ButtonEnable(false);

	mMeteorButton.Button->SetTexture(eButtonState::Normal, "MeteorButton", TEXT("Dia2UI/SkillIcon/IconMeteor.png"));
	mMeteorButton.Button->SetTexture(eButtonState::Normal, "MeteorButton", TEXT("Dia2UI/SkillIcon/IconMeteor.png"));
	mMeteorButton.Button->SetTexture(eButtonState::Hovered, "MeteorButton", TEXT("Dia2UI/SkillIcon/IconMeteor.png"));
	mMeteorButton.Button->SetTexture(eButtonState::Disabled, "MeteorButton", TEXT("Dia2UI/SkillIcon/IconMeteor.png"));
	mMeteorButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mMeteorButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mMeteorButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mMeteorButton.Button->SetTexture(eButtonState::LClicked, "MeteorButton", TEXT("Dia2UI/SkillIcon/IconMeteor.png"));
	mMeteorButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mMeteorButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mMeteorButton.Button->ButtonEnable(false);

	mIceBoltButton.Button->SetTexture(eButtonState::Normal, "IceBoltButton", TEXT("Dia2UI/SkillIcon/IconIceBolt.png"));
	mIceBoltButton.Button->SetTexture(eButtonState::Normal, "IceBoltButton", TEXT("Dia2UI/SkillIcon/IconIceBolt.png"));
	mIceBoltButton.Button->SetTexture(eButtonState::Hovered, "IceBoltButton", TEXT("Dia2UI/SkillIcon/IconIceBolt.png"));
	mIceBoltButton.Button->SetTexture(eButtonState::Disabled, "IceBoltButton", TEXT("Dia2UI/SkillIcon/IconIceBolt.png"));
	mIceBoltButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mIceBoltButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mIceBoltButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mIceBoltButton.Button->SetTexture(eButtonState::LClicked, "IceBoltButton", TEXT("Dia2UI/SkillIcon/IconIceBolt.png"));
	mIceBoltButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mIceBoltButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mIceBoltButton.Button->ButtonEnable(false);

	mFrozenOrbButton.Button->SetTexture(eButtonState::Normal, "FrozenOrbButton", TEXT("Dia2UI/SkillIcon/IconFrozenOrb.png"));
	mFrozenOrbButton.Button->SetTexture(eButtonState::Normal, "FrozenOrbButton", TEXT("Dia2UI/SkillIcon/IconFrozenOrb.png"));
	mFrozenOrbButton.Button->SetTexture(eButtonState::Hovered, "FrozenOrbButton", TEXT("Dia2UI/SkillIcon/IconFrozenOrb.png"));
	mFrozenOrbButton.Button->SetTexture(eButtonState::Disabled, "FrozenOrbButton", TEXT("Dia2UI/SkillIcon/IconFrozenOrb.png"));
	mFrozenOrbButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFrozenOrbButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFrozenOrbButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mFrozenOrbButton.Button->SetTexture(eButtonState::LClicked, "FrozenOrbButton", TEXT("Dia2UI/SkillIcon/IconFrozenOrb.png"));
	mFrozenOrbButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mFrozenOrbButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mFrozenOrbButton.Button->ButtonEnable(false);

	mTeleportButton.Button->SetTexture(eButtonState::Normal, "TeleportButton", TEXT("Dia2UI/SkillIcon/IconTeleport.png"));
	mTeleportButton.Button->SetTexture(eButtonState::Normal, "TeleportButton", TEXT("Dia2UI/SkillIcon/IconTeleport.png"));
	mTeleportButton.Button->SetTexture(eButtonState::Hovered, "TeleportButton", TEXT("Dia2UI/SkillIcon/IconTeleport.png"));
	mTeleportButton.Button->SetTexture(eButtonState::Disabled, "TeleportButton", TEXT("Dia2UI/SkillIcon/IconTeleport.png"));
	mTeleportButton.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mTeleportButton.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mTeleportButton.Button->AddFrameData(eButtonState::Disabled, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	mTeleportButton.Button->SetTexture(eButtonState::LClicked, "TeleportButton", TEXT("Dia2UI/SkillIcon/IconTeleport.png"));
	mTeleportButton.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	mTeleportButton.Button->SetTextureTint(eButtonState::Disabled, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	mTeleportButton.Button->ButtonEnable(false);

	mSkillPointLabelText->SetFont("Dia2Font");
	mSkillPointLabelText->SetAlignH(eTextAlignH::Center);
	mSkillPointLabelText->SetAlignV(eTextAlignV::Middle);
	mSkillPointLabelText->SetColor(1.f, 1.f, 1.f);
	mSkillPointLabelText->SetText(TEXT("Skill Point"));
	mSkillPointLabelText->SetShadowEnable(true);
	mSkillPointLabelText->SetShadowColor(0.f, 0.f, 0.f);
	mSkillPointLabelText->SetShadowOffset(2.f, 2.f);

	mSkillPointText->SetFont("Dia2Font");
	mSkillPointText->SetAlignH(eTextAlignH::Center);
	mSkillPointText->SetAlignV(eTextAlignV::Middle);
	mSkillPointText->SetColor(1.f, 1.f, 1.f);
	mSkillPointText->SetText(TEXT("0"));
	mSkillPointText->SetShadowEnable(true);
	mSkillPointText->SetShadowColor(0.f, 0.f, 0.f);
	mSkillPointText->SetShadowOffset(2.f, 2.f);

	mSkillTree1Text->SetFont("Dia2Font");
	mSkillTree1Text->SetAlignH(eTextAlignH::Center);
	mSkillTree1Text->SetAlignV(eTextAlignV::Middle);
	mSkillTree1Text->SetColor(1.f, 1.f, 1.f);
	mSkillTree1Text->SetShadowEnable(true);
	mSkillTree1Text->SetShadowColor(0.f, 0.f, 0.f);
	mSkillTree1Text->SetShadowOffset(2.f, 2.f);
	mSkillTree1Text->SetFontSize(15.f);
	mSkillTree1Text->SetText(TEXT("Fire"));

	mSkillTree2Text->SetFont("Dia2Font");
	mSkillTree2Text->SetAlignH(eTextAlignH::Center);
	mSkillTree2Text->SetAlignV(eTextAlignV::Middle);
	mSkillTree2Text->SetColor(1.f, 1.f, 1.f);
	mSkillTree2Text->SetShadowEnable(true);
	mSkillTree2Text->SetShadowColor(0.f, 0.f, 0.f);
	mSkillTree2Text->SetShadowOffset(2.f, 2.f);
	mSkillTree2Text->SetFontSize(15.f);
	mSkillTree2Text->SetText(TEXT("Ice"));

	mSkillTree3Text->SetFont("Dia2Font");
	mSkillTree3Text->SetAlignH(eTextAlignH::Center);
	mSkillTree3Text->SetAlignV(eTextAlignV::Middle);
	mSkillTree3Text->SetColor(1.f, 1.f, 1.f);
	mSkillTree3Text->SetShadowEnable(true);
	mSkillTree3Text->SetShadowColor(0.f, 0.f, 0.f);
	mSkillTree3Text->SetShadowOffset(2.f, 2.f);
	mSkillTree3Text->SetFontSize(15.f);
	mSkillTree3Text->SetText(TEXT("Lightning"));

	mFireBoltButton.LevelText->SetFont("Default");
	mFireBoltButton.LevelText->SetFontSize(13.f);
	mFireBoltButton.LevelText->SetAlignH(eTextAlignH::Center);
	mFireBoltButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mFireBoltButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mFireBoltButton.LevelText->SetText(TEXT("0"));
	mFireBoltButton.LevelText->SetShadowEnable(true);
	mFireBoltButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mFireBoltButton.LevelText->SetShadowOffset(2.f, 2.f);
	mFireBallButton.LevelText->SetFont("Default");
	mFireBallButton.LevelText->SetFontSize(13.f);
	mFireBallButton.LevelText->SetAlignH(eTextAlignH::Center);
	mFireBallButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mFireBallButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mFireBallButton.LevelText->SetText(TEXT("0"));
	mFireBallButton.LevelText->SetShadowEnable(true);
	mFireBallButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mFireBallButton.LevelText->SetShadowOffset(2.f, 2.f);
	mBlazeButton.LevelText->SetFont("Default");
	mBlazeButton.LevelText->SetFontSize(13.f);
	mBlazeButton.LevelText->SetAlignH(eTextAlignH::Center);
	mBlazeButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mBlazeButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mBlazeButton.LevelText->SetText(TEXT("0"));
	mBlazeButton.LevelText->SetShadowEnable(true);
	mBlazeButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mBlazeButton.LevelText->SetShadowOffset(2.f, 2.f);
	mMeteorButton.LevelText->SetFont("Default");
	mMeteorButton.LevelText->SetFontSize(13.f);
	mMeteorButton.LevelText->SetAlignH(eTextAlignH::Center);
	mMeteorButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mMeteorButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mMeteorButton.LevelText->SetText(TEXT("0"));
	mMeteorButton.LevelText->SetShadowEnable(true);
	mMeteorButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mMeteorButton.LevelText->SetShadowOffset(2.f, 2.f);
	mIceBoltButton.LevelText->SetFont("Default");
	mIceBoltButton.LevelText->SetFontSize(13.f);
	mIceBoltButton.LevelText->SetAlignH(eTextAlignH::Center);
	mIceBoltButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mIceBoltButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mIceBoltButton.LevelText->SetText(TEXT("0"));
	mIceBoltButton.LevelText->SetShadowEnable(true);
	mIceBoltButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mIceBoltButton.LevelText->SetShadowOffset(2.f, 2.f);
	mFrozenOrbButton.LevelText->SetFont("Default");
	mFrozenOrbButton.LevelText->SetFontSize(13.f);
	mFrozenOrbButton.LevelText->SetAlignH(eTextAlignH::Center);
	mFrozenOrbButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mFrozenOrbButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mFrozenOrbButton.LevelText->SetText(TEXT("0"));
	mFrozenOrbButton.LevelText->SetShadowEnable(true);
	mFrozenOrbButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mFrozenOrbButton.LevelText->SetShadowOffset(2.f, 2.f);
	mTeleportButton.LevelText->SetFont("Default");
	mTeleportButton.LevelText->SetFontSize(13.f);
	mTeleportButton.LevelText->SetAlignH(eTextAlignH::Center);
	mTeleportButton.LevelText->SetAlignV(eTextAlignV::Middle);
	mTeleportButton.LevelText->SetColor(1.f, 1.f, 1.f);
	mTeleportButton.LevelText->SetText(TEXT("0"));
	mTeleportButton.LevelText->SetShadowEnable(true);
	mTeleportButton.LevelText->SetShadowColor(0.f, 0.f, 0.f);
	mTeleportButton.LevelText->SetShadowOffset(2.f, 2.f);

	// CallBack
	mSkillTree1ExitButton->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickExit);
	mSkillTree2ExitButton->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickExit);
	mSkillTree3ExitButton->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickExit);

	mSkillTree1Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickSkillTree1Button);
	mSkillTree2Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickSkillTree2Button);
	mSkillTree3Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickSkillTree3Button);

	mSkillTree2Panel->Enable(false);
	mIceBoltButton.Button->Enable(false);
	mIceBoltButton.LevelText->Enable(false);
	mFrozenOrbButton.Button->Enable(false);
	mFrozenOrbButton.LevelText->Enable(false);
	mSkillTree2ExitButton->Enable(false);

	mSkillTree3Panel->Enable(false);
	mTeleportButton.Button->Enable(false);
	mTeleportButton.LevelText->Enable(false);
	mSkillTree3ExitButton->Enable(false);

	mFireBoltButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickFireBolt);
	mFireBallButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickFireBall);
	mBlazeButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickBlaze);
	mMeteorButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickMeteor);
	mIceBoltButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickIceBolt);
	mFrozenOrbButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickFrozenOrb);
	mTeleportButton.Button->SetLClickCallBack(this, &CD2SkillTreeWindow::onClickTeleport);
}

void CD2SkillTreeWindow::onInit()
{
	std::map<std::string, D2SkillStateInfo>	outInfo;

	mSkill->GetSkillStateInfo(outInfo);

	auto iter = outInfo.find("FireBolt");
	setButton(iter->second, mFireBoltButton);
	iter = outInfo.find("FireBall");
	setButton(iter->second, mFireBallButton);
	iter = outInfo.find("Blaze");
	setButton(iter->second, mBlazeButton);
	iter = outInfo.find("MeteorTarget");
	setButton(iter->second, mMeteorButton);
	iter = outInfo.find("IceBolt");
	setButton(iter->second, mIceBoltButton);
	iter = outInfo.find("FrozenOrb");
	setButton(iter->second, mFrozenOrbButton);
	iter = outInfo.find("Teleport");
	setButton(iter->second, mTeleportButton);
}

void CD2SkillTreeWindow::setButton(const D2SkillStateInfo& info, const SkillButton& button)
{
	if (!info.bLevelUpAvailable)
	{
		return;
	}

	button.Button->ButtonEnable(true);

	char buf[64] = {};
	sprintf_s(buf, "%d", info.Level);
	TCHAR convert[64] = {};
	int length = MultiByteToWideChar(CP_ACP, 0, buf, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, buf, -1, convert, length);
	button.LevelText->SetText(convert);
}

void CD2SkillTreeWindow::onChangeSkillPoint()
{
	int remainSkillPoint = mSkill->GetSkillPoint();
	char buf[32] = {};
	sprintf_s(buf, "%d", remainSkillPoint);
	TCHAR convert[32] = {};
	int length = MultiByteToWideChar(CP_ACP, 0, buf, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, buf, -1, convert, length);
	mSkillPointText->SetText(convert);

	// 스킬포인트가 남아있지 않으면, Status Bar의 스킬트리 오픈 버튼을 Disable한다. 
	if (remainSkillPoint == 0)
	{
		CD2UIManager::GetInst()->GetStatusBarWindow()->OnRemainSkillPointZero();
	}
}

void CD2SkillTreeWindow::onStateChange(D2SkillStateInfo info)
{
	if (info.Name == "FireBolt")
	{
		setButton(info, mFireBoltButton);
	}
	else if (info.Name == "FireBall")
	{
		setButton(info, mFireBallButton);
	}
	else if (info.Name == "Blaze")
	{
		setButton(info, mBlazeButton);
	}
	else if (info.Name == "MeteorTarget")
	{
		setButton(info, mMeteorButton);
	}
	else if (info.Name == "IceBolt")
	{
		setButton(info, mIceBoltButton);
	}
	else if (info.Name == "FrozenOrb")
	{
		setButton(info, mFrozenOrbButton);
	}
	else if (info.Name == "Teleport")
	{
		setButton(info, mTeleportButton);
	}
}

void CD2SkillTreeWindow::onClickSkillTree1Button()
{
	mSkillTree1Panel->Enable(true);
	mFireBoltButton.Button->Enable(true);
	mFireBoltButton.LevelText->Enable(true);
	mFireBallButton.Button->Enable(true);
	mFireBallButton.LevelText->Enable(true);
	mBlazeButton.Button->Enable(true);
	mBlazeButton.LevelText->Enable(true);
	mMeteorButton.Button->Enable(true);
	mMeteorButton.LevelText->Enable(true);
	mSkillTree1ExitButton->Enable(true);
    
	mSkillTree2Panel->Enable(false);
	mIceBoltButton.Button->Enable(false);
	mIceBoltButton.LevelText->Enable(false);
	mFrozenOrbButton.Button->Enable(false);
	mFrozenOrbButton.LevelText->Enable(false);
	mSkillTree2ExitButton->Enable(false);

	mSkillTree3Panel->Enable(false);
	mSkillTree3ExitButton->Enable(false);
	mTeleportButton.Button->Enable(false);
	mTeleportButton.LevelText->Enable(false);

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillTreeWindow::onClickSkillTree2Button()
{
	mSkillTree1Panel->Enable(false);
	mFireBoltButton.Button->Enable(false);
	mFireBoltButton.LevelText->Enable(false);
	mFireBallButton.Button->Enable(false);
	mFireBallButton.LevelText->Enable(false);
	mBlazeButton.Button->Enable(false);
	mBlazeButton.LevelText->Enable(false);
	mMeteorButton.Button->Enable(false);
	mMeteorButton.LevelText->Enable(false);
	mSkillTree1ExitButton->Enable(false);
    
	mSkillTree2Panel->Enable(true);
	mIceBoltButton.Button->Enable(true);
	mIceBoltButton.LevelText->Enable(true);
	mFrozenOrbButton.Button->Enable(true);
	mFrozenOrbButton.LevelText->Enable(true);
	mSkillTree2ExitButton->Enable(true);

	mSkillTree3Panel->Enable(false);
	mSkillTree3ExitButton->Enable(false);
	mTeleportButton.Button->Enable(false);
	mTeleportButton.LevelText->Enable(false);
	
	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillTreeWindow::onClickSkillTree3Button()
{
	mSkillTree1Panel->Enable(false);
	mFireBoltButton.Button->Enable(false);
	mFireBoltButton.LevelText->Enable(false);
	mFireBallButton.Button->Enable(false);
	mFireBallButton.LevelText->Enable(false);
	mBlazeButton.Button->Enable(false);
	mBlazeButton.LevelText->Enable(false);
	mMeteorButton.Button->Enable(false);
	mMeteorButton.LevelText->Enable(false);
	mSkillTree1ExitButton->Enable(false);
    
	mSkillTree2Panel->Enable(false);
	mIceBoltButton.Button->Enable(false);
	mIceBoltButton.LevelText->Enable(false);
	mFrozenOrbButton.Button->Enable(false);
	mFrozenOrbButton.LevelText->Enable(false);
	mSkillTree2ExitButton->Enable(false);

	mSkillTree3Panel->Enable(true);
	mSkillTree3ExitButton->Enable(true);
	mTeleportButton.Button->Enable(true);
	mTeleportButton.LevelText->Enable(true);
	
	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillTreeWindow::onClickFireBolt()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree1, "FireBolt"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickFireBall()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree1, "FireBall"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickBlaze()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree1, "Blaze"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickMeteor()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree1, "MeteorTarget"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickIceBolt()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree2, "IceBolt"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickFrozenOrb()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree2, "FrozenOrb"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickTeleport()
{
	if (mSkill->LevelUp(eD2SkillTreeNo::Tree3, "Teleport"))
	{
		onChangeSkillPoint();

		mViewport->GetScene()->GetResource()->SoundPlay("Button");
	}
}

void CD2SkillTreeWindow::onClickExit()
{
	CD2UIManager::GetInst()->CloseMenu();
}
