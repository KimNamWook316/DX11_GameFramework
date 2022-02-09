#include "CollisionProfileWindow.h"
#include "Collision/CollisionManager.h"
#include "IMGUIListBox.h"
#include "IMGUITextInput.h"
#include "IMGUICheckBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIButton.h"
#include "IMGUIText.h"
#include "IMGUISeperator.h"
#include "IMGUISameLine.h"
#include "EditorInfo.h"

CCollisionProfileWindow::CCollisionProfileWindow()	:
	mProfileList(nullptr),
	mChannelList(nullptr),
	mProfileNameInput(nullptr),
	mCreateProfileButton(nullptr),
	mProfileChannel(nullptr),
	mInteractionEdit(nullptr),
	mSaveButton(nullptr)
{
}

CCollisionProfileWindow::~CCollisionProfileWindow()
{
}

bool CCollisionProfileWindow::Init()
{
	// Create Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("Collision Profile List");
	mProfileList = AddWidget<CIMGUIListBox>("Collision Profile");
	mChannelList = AddWidget<CIMGUIComboBox>("Channel");
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	mProfileNameInput = AddWidget<CIMGUITextInput>("Profile Name");
	line = AddWidget<CIMGUISameLine>("line");
	mCreateProfileButton = AddWidget<CIMGUIButton>("Create Profile", 0.f, 0.f);
	text = AddWidget<CIMGUIText>("text");
	text->SetText("Profile Channel");
	mProfileChannel = AddWidget<CIMGUITextInput>("Profile Channel");
	text = AddWidget<CIMGUIText>("text");
	text->SetText("Interaction");
	mInteractionEdit = AddWidget<CIMGUICheckBox>("Interaction");
	mSaveButton = AddWidget<CIMGUIButton>("Save", 0.f, 0.f);

	// Initial Value
	std::vector<std::string> profileNames;
	CCollisionManager::GetInst()->GetProfileNames(profileNames);
	size_t size = profileNames.size();
	for (size_t i = 0; i < size; ++i)
	{
		mProfileList->AddItem(profileNames[i]);
	}

	for (size_t i = 0; i < (size_t)eCollisionChannel::Max; ++i)
	{
		mChannelList->AddItem(CollisionChannelToString((eCollisionChannel)i));
	}

	mProfileNameInput->SetHintText("Profile Name");

	mProfileChannel->ReadOnly(true);

	for (size_t i = 0; i < (size_t)eCollisionChannel::Max; ++i)
	{
		mInteractionEdit->AddCheckInfo(CollisionChannelToString((eCollisionChannel)i).c_str());
	}

	// CallBack
	mProfileList->SetSelectCallBack(this, &CCollisionProfileWindow::OnSelectList);
	mCreateProfileButton->SetClickCallBack(this, &CCollisionProfileWindow::OnClickCreate);
	mInteractionEdit->SetCallBackIdx(this, &CCollisionProfileWindow::OnCheckInteraction);
	mSaveButton->SetClickCallBack(this, &CCollisionProfileWindow::OnClickSave);
	return true;
}

void CCollisionProfileWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CCollisionProfileWindow::OnSelectList(int idx, const char* label)
{
	CollisionProfile* profile = CCollisionManager::GetInst()->FindProfile(label);
	
	mProfileChannel->SetText(CollisionChannelToString(profile->Channel).c_str());

	size_t size = profile->vecInteraction.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (eCollisionInteraction::Collision == profile->vecInteraction[i])
		{
			mInteractionEdit->SetCheck((int)i, true);
		}
		else
		{
			mInteractionEdit->SetCheck((int)i, false);
		}
	}
}

void CCollisionProfileWindow::OnClickCreate()
{
	if (mProfileNameInput->IsEmpty())
	{
		return;
	}
	
	if (mChannelList->GetSelectIndex() == -1)
	{
		return;
	}

	CCollisionManager::GetInst()->CreateProfile(mProfileNameInput->GetTextUTF8(),
		(eCollisionChannel)mChannelList->GetSelectIndex(), true);
}

void CCollisionProfileWindow::OnCheckInteraction(int idx, bool bCheck)
{
	if (-1 == mProfileList->GetSelectIndex())
	{
		return;
	}
	
	eCollisionInteraction interaction = bCheck ? eCollisionInteraction::Collision : eCollisionInteraction::Ignore;

	CCollisionManager::GetInst()->SetCollisionState(mProfileList->GetSelectItem(),
		(eCollisionChannel)idx, interaction);
}

void CCollisionProfileWindow::OnClickSave()
{
	// TODO : Save as csv
}
