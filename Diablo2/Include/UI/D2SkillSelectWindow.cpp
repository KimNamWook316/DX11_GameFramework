#include "D2SkillSelectWindow.h"
#include "../Component/D2PlayerSkillComponent.h"
#include "Resource/Excel/ExcelData.h"
#include "Scene/Viewport.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "../Component/D2UIManager.h"
#include "../Component/D2DataManager.h"
#include "../UI/D2StatusBarWindow.h"

CD2SkillSelectWindow::CD2SkillSelectWindow()	:
	mSkill(nullptr)
{
}

CD2SkillSelectWindow::CD2SkillSelectWindow(const CD2SkillSelectWindow& window)	:
	CWidgetWindow(window)
{
}

CD2SkillSelectWindow::~CD2SkillSelectWindow()
{
}

bool CD2SkillSelectWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2SkillSelectWindow::Start()
{
	CWidgetWindow::Start();

	mSkill = mViewport->GetScene()->GetPlayerObj()->FindObjectComponentFromType<CD2PlayerSkillComponent>();
	mSkill->SetStateChangeCallBackAll(this, &CD2SkillSelectWindow::onSkillLevelUp);

	for (int i = 0; i < (int)eD2SkillTreeNo::Max; ++i)
	{
		mButtonStartPos[i] = Vector2(mSize.x - 48.f, i * 48.f);
	}

	onInit();
}

void CD2SkillSelectWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2SkillSelectWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2SkillSelectWindow::Render()
{
	CWidgetWindow::Render();
}

CD2SkillSelectWindow* CD2SkillSelectWindow::Clone()
{
	return new CD2SkillSelectWindow(*this);
}

void CD2SkillSelectWindow::AddButton(const D2SkillStateInfo& info)
{
	if (info.Level == 0)
	{
		return;
	}

	D2SkillSelectButton button;
	button.Button = CreateWidget<CButton>(info.Name + "SelectButton");
	CTexture* buttonTexture = CD2DataManager::GetInst()->GetSkillIconTexture(info.Name);

	button.Button->SetTexture(eButtonState::Normal, buttonTexture);
	button.Button->SetTexture(eButtonState::Hovered, buttonTexture);
	button.Button->SetTexture(eButtonState::LClicked, buttonTexture);
	button.Button->AddFrameData(eButtonState::Normal, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	button.Button->AddFrameData(eButtonState::Hovered, Vector2(0.f, 0.f), Vector2(48.f, 48.f));
	button.Button->AddFrameData(eButtonState::LClicked, Vector2(49.f, 0.f), Vector2(48.f, 48.f));
	button.Button->SetSize(48.f, 48.f);
	button.Button->SetPos(mButtonStartPos[(int)info.TreeNo]);

	mButtonStartPos[(int)info.TreeNo] += Vector2(-48.f, 0.f);

	setCallBack(button.Button, info.Name);
	mSkillSelectButton[(int)info.TreeNo].push_back(button);
}

void CD2SkillSelectWindow::onInit()
{
	std::map<std::string, D2SkillStateInfo> outInfo;
	mSkill->GetSkillStateInfo(outInfo);

	auto iter = outInfo.begin();
	auto iterEnd = outInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		AddButton(iter->second);
	}
}

void CD2SkillSelectWindow::setCallBack(CButton* button, const std::string& skillName)
{
	if (skillName == "FireBolt")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickFireBolt);
	}
	else if (skillName == "FireBall")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickFireBall);
	}
	else if (skillName == "Blaze")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickBlaze);
	}
	else if (skillName == "MeteorTarget")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickMeteor);
	}
	else if (skillName == "IceBolt")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickIceBolt);
	}
	else if (skillName == "FrozenOrb")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickFrozenOrb);
	}
	else if (skillName == "Teleport")
	{
		button->SetLClickCallBack(this, &CD2SkillSelectWindow::onClickTeleport);
	}
}

void CD2SkillSelectWindow::onSkillLevelUp(D2SkillStateInfo info)
{
	// 처음 레벨업 된 경우에 버튼 추가
	if (info.bFirstLevelUp)
	{
		AddButton(info);
	}
}

void CD2SkillSelectWindow::onClickFireBolt()
{
	mSkill->SetRSkill("FireBolt");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("FireBolt");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillSelectWindow::onClickFireBall()
{
	mSkill->SetRSkill("FireBall");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("FireBall");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillSelectWindow::onClickBlaze()
{
	mSkill->SetRSkill("Blaze");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("Blaze");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillSelectWindow::onClickMeteor()
{
	mSkill->SetRSkill("MeteorTarget");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("MeteorTarget");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillSelectWindow::onClickIceBolt()
{
	mSkill->SetRSkill("IceBolt");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("IceBolt");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillSelectWindow::onClickFrozenOrb()
{
	mSkill->SetRSkill("FrozenOrb");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("FrozenOrb");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}

void CD2SkillSelectWindow::onClickTeleport()
{
	mSkill->SetRSkill("Teleport");
	CD2UIManager::GetInst()->GetStatusBarWindow()->SetRSkillButtonTexture("Teleport");

	mViewport->GetScene()->GetResource()->SoundPlay("Button");
}
