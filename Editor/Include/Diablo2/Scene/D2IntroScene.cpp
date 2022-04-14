#include "D2IntroScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Resource/Excel/ExcelData.h"
#include "Resource/ResourceManager.h"

CD2IntroScene::CD2IntroScene()
{
}

CD2IntroScene::~CD2IntroScene()
{
	mIntroMusic->Stop();
	CResourceManager::GetInst()->ReleaseSound("IntroMusic");
}

bool CD2IntroScene::Init()
{
	if (!loadWindow())
	{
		return false;
	}
	if (!loadSound())
	{
		return false;
	}

	mIntroMusic->Play();

	return true;
}

bool CD2IntroScene::loadWindow()
{
	std::string name;
	CResourceManager::GetInst()->LoadCSV(name, "D2WindowInfo.csv");

	CExcelData* csv = CResourceManager::GetInst()->FindCSV(name);

	std::vector<std::string>* row = csv->GetRow("Intro");
	mIntroWindow = mScene->GetViewport()->CreateWidgetWindow<CD2IntroWindow>("Intro");
	Vector2 size = Vector2(std::stof((*row)[1]), std::stof((*row)[2]));
	Vector2 pos = Vector2(std::stof((*row)[3]), std::stof((*row)[4]));
	mIntroWindow->SetSize(size);
	mIntroWindow->SetPos(pos);

	CResourceManager::GetInst()->LoadCSV(name, "D2WidgetInfo.csv");
	CExcelData* widgetCSV = CResourceManager::GetInst()->FindCSV(name);
	mIntroWindow->MakeWidget(widgetCSV);

	return true;
}

bool CD2IntroScene::loadSound()
{
	CResourceManager::GetInst()->LoadSound("Master", true, "IntroMusic",
		"Diablo2/title.mp3");

	mIntroMusic = CResourceManager::GetInst()->FindSound("IntroMusic");
	return true;
}
