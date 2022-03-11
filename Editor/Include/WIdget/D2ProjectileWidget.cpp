#include "D2ProjectileWidget.h"
#include "../Diablo2/Component/D2ProjectTile.h"
#include "../Diablo2/Component/D2DataManager.h"
#include "IMGUISeperator.h"
#include "IMGUIText.h"
#include "IMGUIComboBox.h"

CD2ProjectileWidget::CD2ProjectileWidget()	:
	mSkillInfo(nullptr)
{
}

CD2ProjectileWidget::~CD2ProjectileWidget()
{
}

bool CD2ProjectileWidget::Init()
{
	CComponentControlWidget::Init();
 //	// widget
 //	CIMGUIText* text = AddWidget<CIMGUIText>("text");
 //	text->SetText("Projectile");
 //	mSkillInfo = AddWidget<CIMGUIComboBox>("Skill Name");
 //	AddWidget<CIMGUISeperator>("sep");
 //	
 //	// initial value
 //	std::vector<std::string> outName;
 //	CD2DataManager::GetInst()->GetSkillNames(outName);
 //	size_t size = outName.size();
 //	for (size_t i = 0; i < size; ++i)
 //	{
 //		mSkillInfo->AddItem(outName[i]);
 //	}
 //
 //	D2SkillInfo info = static_cast<CD2Projectile*>(mComponent)->GetInfo();
 //	if (!info.Name.empty())
 //	{
 //		mSkillInfo->SetCurrentItem(info.Name);
 //	}
 //	
 //	// callback
 //	mSkillInfo->SetSelectCallBack(this, &CD2ProjectileWidget::OnSelectSkillInfo);
 //
	return true;
}

void CD2ProjectileWidget::OnSelectSkillInfo(int idx, const char* label)
{
 //	static_cast<CD2Projectile*>(mComponent)->SetInfo(label);
}
