#include "StateControlWidget.h"
#include "Component/StateComponent.h"
#include "IMGUIText.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "../Diablo2/D2Info.h"
#include "../Diablo2/D2Util.h"
#include "Scene/SceneManager.h"

CStateControlWidget::CStateControlWidget()	:
	mInitialStateComoboBox(nullptr),
	mSetInitialStateButton(nullptr)
{
}

CStateControlWidget::~CStateControlWidget()
{
}

bool CStateControlWidget::Init()
{
	// Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("State Component");
	mInitialStateComoboBox = AddWidget<CIMGUIComboBox>("Intial State");
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	mSetInitialStateButton = AddWidget<CIMGUIButton>("Set", 0.f, 0.f);
	CIMGUISeperator* sep = AddWidget<CIMGUISeperator>("sep");

	// Initial Value
	for(int i = 0; i < (int)eD2StateType::MAX; ++i)
	{
		mInitialStateComoboBox->AddItem(CD2Util::StateEnumToString((eD2StateType)i));
	}

	CStateComponent* stateComp = static_cast<CStateComponent*>(mComponent);
	if (!stateComp->IsStateEmpty())
	{
		mInitialStateComoboBox->SetCurrentItem(CD2Util::StateTypeToString(stateComp->GetInitialStateType()));
	}
	
	// CallBack
	mSetInitialStateButton->SetClickCallBack(this, &CStateControlWidget::OnClickSetInitialState);

	return true;
}

void CStateControlWidget::OnClickSetInitialState()
{
	if (-1 == mInitialStateComoboBox->GetSelectIndex())
	{
		return;
	}

	static_cast<CStateComponent*>(mComponent)->Clear();

	size_t stateType = CD2Util::StateEnumToStateType(CD2Util::StringToStateEnum(mInitialStateComoboBox->GetSelectItem()));
	CSceneManager::GetInst()->CallCreateStateFunction((CStateComponent*)mComponent, stateType);
}
