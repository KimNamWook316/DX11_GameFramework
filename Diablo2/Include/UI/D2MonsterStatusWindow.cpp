#include "D2MonsterStatusWindow.h"
#include "Resource/Excel/ExcelData.h"
#include "Scene/Viewport.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"

CD2MonsterStatusWindow::CD2MonsterStatusWindow()
{
}

CD2MonsterStatusWindow::CD2MonsterStatusWindow(const CD2MonsterStatusWindow& window)	:
	CWidgetWindow(window)
{
}

CD2MonsterStatusWindow::~CD2MonsterStatusWindow()
{
}

bool CD2MonsterStatusWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2MonsterStatusWindow::Start()
{
	CWidgetWindow::Start();
}

void CD2MonsterStatusWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2MonsterStatusWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2MonsterStatusWindow::Render()
{
	CWidgetWindow::Render();
}

CD2MonsterStatusWindow* CD2MonsterStatusWindow::Clone()
{
	return new CD2MonsterStatusWindow(*this);
}

void CD2MonsterStatusWindow::MakeWidget(CExcelData* data)
{
	// Create Widget
	mHealthBarBase = CreateWidget<CImage>("MonsterHealthBarBase");
	mHealthBar = CreateWidget<CProgressBar>("MonsterHealthBar");
	mNameText = CreateWidget<CText>("MonsterHealthBarText");

	// Position, Size
	std::vector<std::string>* row = data->GetRow("MonsterHealthBarBase");
	mHealthBarBase->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mHealthBarBase->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("MonsterHealthBar");
	mHealthBar->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mHealthBar->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));
	row = data->GetRow("MonsterHealthBarText");
	mNameText->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mNameText->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
	mHealthBarBase->SetTextureTint(Vector4(0.f, 0.f, 0.f, 0.7f));
	mHealthBar->SetTextureTint(Vector4(0.7f, 0.f, 0.f, 0.7f));
	mHealthBar->SetDir(eProgressBarDir::RightToLeft);
	mNameText->SetFont("Dia2Font");
	mNameText->SetColor(1.f, 0.95f, 0.83f);
	mNameText->SetAlignH(eTextAlignH::Center);
	mNameText->SetAlignV(eTextAlignV::Middle);
	mNameText->SetFontSize(12.f);
	mNameText->SetText(TEXT("Empty"));
}

void CD2MonsterStatusWindow::UpdateState(const std::string& monsterName, const float percent)
{
	char buf[128] = {};
	strcpy_s(buf, monsterName.c_str());
	TCHAR bufConvert[128] = {};
	int length = MultiByteToWideChar(CP_ACP, 0, buf, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, buf, length, bufConvert, length);

	mNameText->SetText(bufConvert);
	mHealthBar->SetPercent(percent);
}
