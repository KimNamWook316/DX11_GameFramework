#include "D2UIManager.h"
#include "../UI/D2InventoryWindow.h"
#include "../UI/D2StatusBarWindow.h"
#include "../UI/D2MenuBorder.h"
#include "Resource/ResourceManager.h"
#include "Resource/Excel/ExcelData.h"
#include "Scene/SceneManager.h"
#include "Input.h"

DEFINITION_SINGLE(CD2UIManager);

CD2UIManager::CD2UIManager()	:
	mStatusBar(nullptr),
	mMenuBorder(nullptr),
	mInven(nullptr)
{
}

CD2UIManager::~CD2UIManager()
{
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
}

void CD2UIManager::OpenInventory()
{
	mMenuBorder->Enable(true);
	mInven->Enable(true);
}

void CD2UIManager::CloseInventory()
{
	mMenuBorder->Enable(false);
	mInven->Enable(false);
}

void CD2UIManager::ToggleInventory(float deltaTime)
{
	if (mInven->IsEnable())
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
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
	mInven = CSceneManager::GetInst()->GetScene()->GetViewport()->CreateWidgetWindow<CD2InventoryWindow>("Inventory");
	size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mInven->SetSize(size);
	mInven->SetPos(pos);

	// zorder
	mMenuBorder->SetZorder(1);
	mStatusBar->SetZorder(2);
	mInven->SetZorder(3);

	CResourceManager::GetInst()->DeleteCSV(name);
}

void CD2UIManager::makeWidget()
{
	std::string name;
	CResourceManager::GetInst()->LoadCSV(name, "D2WidgetInfo.csv");
	CExcelData* csv = CResourceManager::GetInst()->FindCSV(name);

	mInven->MakeWidget(csv);
	mMenuBorder->MakeWidget(csv);

	CResourceManager::GetInst()->DeleteCSV(name);
}
