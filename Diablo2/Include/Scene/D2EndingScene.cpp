#include "D2EndingScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Resource/Excel/ExcelData.h"
#include "Resource/ResourceManager.h"
#include "Engine.h"
#include "Input.h"

CD2EndingScene::CD2EndingScene()
{
}

CD2EndingScene::~CD2EndingScene()
{
}

bool CD2EndingScene::Init()
{
	if (!loadWindow())
	{
		return false;
	}

	createKey();

	return true;
}

bool CD2EndingScene::loadWindow()
{
	std::string name;
	CResourceManager::GetInst()->LoadCSV(name, "D2WindowInfo.csv");

	CExcelData* csv = CResourceManager::GetInst()->FindCSV(name);

	std::vector<std::string>* row = csv->GetRow("Ending");
	mEndingWindow = mScene->GetViewport()->CreateWidgetWindow<CD2EndingWidget>("Ending");
	Vector2 size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	Vector2 pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mEndingWindow->SetSize(size);
	mEndingWindow->SetPos(pos);

	CResourceManager::GetInst()->LoadCSV(name, "D2WidgetInfo.csv");
	CExcelData* widgetCSV = CResourceManager::GetInst()->FindCSV(name);
	mEndingWindow->MakeWidget(widgetCSV);
	return true;
}

void CD2EndingScene::createKey()
{
	CInput::GetInst()->CreateKey("Exit", VK_ESCAPE);
	CInput::GetInst()->SetKeyCallBack("Exit", eKeyState::KeyState_Down, this, &CD2EndingScene::onESCDown);
}

void CD2EndingScene::onESCDown(float deltaTime)
{
	CEngine::GetInst()->Exit();
}
