#include "D2CharInfoWidget.h"
#include "IMGUISeperator.h"
#include "IMGUIText.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputFloat2.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "../Diablo2/Component/D2DataManager.h"
#include "../Diablo2/Component/D2CharacterInfoComponent.h"
#include "../Diablo2/D2Util.h"

CD2CharInfoWidget::CD2CharInfoWidget()	:
	mDataSet(nullptr),
	mName(nullptr),
	mSpeed(nullptr),
	mMaxSpeed(nullptr),
	mDir(nullptr),
	mSpriteDir(nullptr)
{
}

CD2CharInfoWidget::~CD2CharInfoWidget()
{
}

bool CD2CharInfoWidget::Init()
{
	D2CharInfo info = static_cast<CD2CharacterInfoComponent*>(mComponent.Get())->GetInfo();

	// Widget
	CIMGUIText* text =AddWidget<CIMGUIText>("text");
	text->SetText("D2CharInfo");
	mDataSet = AddWidget<CIMGUIComboBox>("Character");
	text = AddWidget<CIMGUIText>("text");
	text->SetText("Name : ");
	AddWidget<CIMGUISameLine>("line");
	mName = AddWidget<CIMGUIText>("text");
	mName->SetText(info.Name.c_str());
	mSpeed = AddWidget<CIMGUIInputFloat>("Speed");
	mMaxSpeed = AddWidget<CIMGUIInputFloat>("Max Speed");
	mDir = AddWidget<CIMGUIInputFloat2>("Direction");
	mSpriteDir = AddWidget<CIMGUITextInput>("SpriteDir");
	AddWidget<CIMGUISeperator>("sep");

	// Initial Value
	std::vector<std::string> outNames;
	CD2DataManager::GetInst()->GetCharacterNames(outNames);

	size_t size = outNames.size();
	for (int i = 0; i < size; ++i)
	{
		mDataSet->AddItem(outNames[i]);
	}
	mDataSet->SetCurrentItem(info.Name);
	mName->SetName(info.Name);
	mSpeed->SetVal(info.Speed);
	mMaxSpeed->SetVal(info.MaxSpeed);
	mDir->SetVal(info.Dir.x, info.Dir.y);
	mSpriteDir->SetText(CD2Util::SpriteDirToString(info.eSpriteDir).c_str());

	// CallBack
	mDataSet->SetSelectCallBack(this, &CD2CharInfoWidget::OnSelectDataSet);

	return true;
}

void CD2CharInfoWidget::OnSelectDataSet(int idx, const char* label)
{
	static_cast<CD2CharacterInfoComponent*>(mComponent.Get())->SetData(label);
	D2CharInfo info = static_cast<CD2CharacterInfoComponent*>(mComponent.Get())->GetInfo();
	
	mDataSet->SetCurrentItem(info.Name);
	mName->SetName(info.Name);
	mSpeed->SetVal(info.Speed);
	mMaxSpeed->SetVal(info.MaxSpeed);
	mDir->SetVal(info.Dir.x, info.Dir.y);
	mSpriteDir->SetText(CD2Util::SpriteDirToString(info.eSpriteDir).c_str());
}
