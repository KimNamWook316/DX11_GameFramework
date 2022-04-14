#include "D2EndingWidget.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Resource/Excel/ExcelData.h"

CD2EndingWidget::CD2EndingWidget()
{
}

CD2EndingWidget::CD2EndingWidget(const CD2EndingWidget& window)	:
	CWidgetWindow(window)
{
}

CD2EndingWidget::~CD2EndingWidget()
{
}

bool CD2EndingWidget::Init()
{
	CWidgetWindow::Init();

	return true;
}

void CD2EndingWidget::Start()
{
	CWidgetWindow::Start();
}

void CD2EndingWidget::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2EndingWidget::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2EndingWidget::Render()
{
	CWidgetWindow::Render();
}

CD2EndingWidget* CD2EndingWidget::Clone()
{
	return new CD2EndingWidget(*this);
}

void CD2EndingWidget::MakeWidget(CExcelData* data)
{
	// Craete
	mEndingImage = CreateWidget<CImage>("EndingImage");

	// Pos, Size
	std::vector<std::string>* row = data->GetRow("EndingImage");
	mEndingImage->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mEndingImage->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
	mEndingImage->SetTexture("EndingImage", TEXT("Dia2UI/EndingScene.png"));
}
