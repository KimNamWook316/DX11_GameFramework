#include "D2UIManager.h"
#include "../UI/D2InventoryWindow.h"
#include "../UI/D2StatusBarWindow.h"
#include "../UI/D2MenuBorder.h"
#include "../UI/D2SkillTreeWindow.h"
#include "../UI/D2SkillSelectWindow.h"
#include "../UI/D2MonsterStatusWindow.h"
#include "D2ClickableUIComponent.h"
#include "Resource/ResourceManager.h"
#include "Resource/Excel/ExcelData.h"
#include "Scene/SceneManager.h"
#include "Input.h"

DEFINITION_SINGLE(CD2UIManager);

CD2UIManager::CD2UIManager()	:
	mCurOpenWindow(nullptr),
	mPrevOpenWindow(nullptr),
	mStatusBar(nullptr),	
	mMenuBorder(nullptr),
	mMonsterStatus(nullptr),
	mbTABDown(false)
{
}

CD2UIManager::~CD2UIManager()
{
	CInput::GetInst()->DeleteCallBack("ToggleInventory", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("ToggleSkillTree", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("CloseMenu", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("ClickableUI", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("ClickableUI", eKeyState::KeyState_Up);
}

bool CD2UIManager::Init()
{
	return true;
}

void CD2UIManager::Start()
{
	makeWindow();
	makeWidget();

	CInput::GetInst()->SetKeyCallBack("ToggleInventory", eKeyState::KeyState_Down, this, &CD2UIManager::ToggleInventory);
	CInput::GetInst()->SetKeyCallBack("ToggleSkillTree", eKeyState::KeyState_Down, this, &CD2UIManager::ToggleSkillTree);
	CInput::GetInst()->SetKeyCallBack("CloseMenu", eKeyState::KeyState_Down, this, &CD2UIManager::OnESCDown);
	CInput::GetInst()->SetKeyCallBack("ClickableUI", eKeyState::KeyState_Down, this, &CD2UIManager::OnTABDown);
	CInput::GetInst()->SetKeyCallBack("ClickableUI", eKeyState::KeyState_Up, this, &CD2UIManager::OnTABUp);

	mMenuBorder->Enable(false);
	mMenuWindow[(int)eD2MenuWindowType::Inventory]->Enable(false);
	mMenuWindow[(int)eD2MenuWindowType::SkillTree]->Enable(false);
	mMenuWindow[(int)eD2MenuWindowType::SkillSelect]->Enable(false);
	mMonsterStatus->Enable(false);
}

CD2InventoryWindow* CD2UIManager::GetInventoryWindow()
{
	return static_cast<CD2InventoryWindow*>(mMenuWindow[(int)eD2MenuWindowType::Inventory]);
}

void CD2UIManager::OpenMenu(eD2MenuWindowType type)
{
	if (type != eD2MenuWindowType::SkillSelect)
	{
		mMenuBorder->Enable(true);
	}
	else
	{
		mMenuBorder->Enable(false);
	}

	if (mPrevOpenWindow)
	{
		mPrevOpenWindow->Enable(false);
	}

	mCurOpenWindow = mMenuWindow[(int)type];
	mCurOpenWindow->Enable(true);
	mPrevOpenWindow = mCurOpenWindow;
}

void CD2UIManager::CloseMenu()
{
	if (mMenuBorder)
	{
		mMenuBorder->Enable(false);
	}
	if (mCurOpenWindow)
	{
		mCurOpenWindow->Enable(false);
	}
}

void CD2UIManager::ToggleMenu(eD2MenuWindowType type)
{
	if (mMenuWindow[(int)type]->IsEnable())
	{
		CloseMenu();
	}
	else
	{
		CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("WindowOpen");
		OpenMenu(type);
	}
}

void CD2UIManager::ToggleInventory(float deltaTime)
{
	ToggleMenu(eD2MenuWindowType::Inventory);
}

void CD2UIManager::ToggleSkillTree(float deltaTime)
{
	ToggleMenu(eD2MenuWindowType::SkillTree);
}

void CD2UIManager::EnableMonsterStateWindow(bool bEnable)
{
	if (mMonsterStatus)
	{
		mMonsterStatus->Enable(bEnable);
	}
}

void CD2UIManager::UpdateMonsterStateWindow(const std::string& monsterName, const float hpPercent)
{
	if (mMonsterStatus)
	{
		mMonsterStatus->UpdateState(monsterName, hpPercent);
	}
}

void CD2UIManager::EnableClickableUI(bool bEnable)
{
	auto iter = mClickableUIList.begin();
	auto iterEnd = mClickableUIList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderEnable(bEnable);
	}
}

void CD2UIManager::OnESCDown(float deltaTime)
{
	CloseMenu();
}

void CD2UIManager::OnTABDown(float deltaTime)
{
	mbTABDown = true;
	EnableClickableUI(true);
}

void CD2UIManager::OnTABUp(float deltaTime)
{
	mbTABDown = false;
	EnableClickableUI(false);
}

bool CD2UIManager::DeleteClickableUI(CD2ClickableUIComponent* ui)
{
	auto iter = mClickableUIList.begin();
	auto iterEnd = mClickableUIList.end();

	for (; iter != iterEnd; ++iter)
	{
		mClickableUIList.erase(iter);
		return true;
	}

	return false;
}

void CD2UIManager::makeWindow()
{
	std::string name;
	CResourceManager::GetInst()->LoadCSV(name, "D2WindowInfo.csv");

	CExcelData* csv = CResourceManager::GetInst()->FindCSV(name);

	std::vector<std::string>* row = csv->GetRow("StatusBar");
	mStatusBar = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2StatusBarWindow>("StatusBar");
	Vector2 size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	Vector2 pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mStatusBar->SetSize(size);
	mStatusBar->SetPos(pos);

	row = csv->GetRow("MenuBorder");
	mMenuBorder = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2MenuBorder>("MenuBorder");
	size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mMenuBorder->SetSize(size);
	mMenuBorder->SetPos(pos);

	row = csv->GetRow("Inventory");
	mMenuWindow[(int)eD2MenuWindowType::Inventory] = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2InventoryWindow>("Inventory");
	size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mMenuWindow[(int)eD2MenuWindowType::Inventory]->SetSize(size);
	mMenuWindow[(int)eD2MenuWindowType::Inventory]->SetPos(pos);

	row = csv->GetRow("SkillTree");
	mMenuWindow[(int)eD2MenuWindowType::SkillTree] = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2SkillTreeWindow>("SkillTree");
	size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mMenuWindow[(int)eD2MenuWindowType::SkillTree]->SetSize(size);
	mMenuWindow[(int)eD2MenuWindowType::SkillTree]->SetPos(pos);

	row = csv->GetRow("SkillSelect");
	mMenuWindow[(int)eD2MenuWindowType::SkillSelect] = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2SkillSelectWindow>("SkillSelect");
	size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mMenuWindow[(int)eD2MenuWindowType::SkillSelect]->SetSize(size);
	mMenuWindow[(int)eD2MenuWindowType::SkillSelect]->SetPos(pos);

	row = csv->GetRow("MonsterStatus");
	mMonsterStatus = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2MonsterStatusWindow>("MonsterStatus");
	size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mMonsterStatus->SetSize(size);
	mMonsterStatus->SetPos(pos);
	
	// zorder
	mMenuBorder->SetZorder(1);
	mStatusBar->SetZorder(2);
	mMenuWindow[(int)eD2MenuWindowType::Inventory]->SetZorder(3);
	mMenuWindow[(int)eD2MenuWindowType::SkillTree]->SetZorder(3);

	CResourceManager::GetInst()->DeleteCSV(name);
}

void CD2UIManager::makeWidget()
{
	std::string name;
	CResourceManager::GetInst()->LoadCSV(name, "D2WidgetInfo.csv");
	CExcelData* csv = CResourceManager::GetInst()->FindCSV(name);

	static_cast<CD2InventoryWindow*>(mMenuWindow[(int)eD2MenuWindowType::Inventory])->MakeWidget(csv);
	mMenuBorder->MakeWidget(csv);
	mStatusBar->MakeWidget(csv);
	mMonsterStatus->MakeWidget(csv);
	static_cast<CD2SkillTreeWindow*>(mMenuWindow[(int)eD2MenuWindowType::SkillTree])->MakeWidget(csv);

	CResourceManager::GetInst()->DeleteCSV(name);
}
