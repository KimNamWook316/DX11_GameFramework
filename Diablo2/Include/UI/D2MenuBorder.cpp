#include "D2MenuBorder.h"
#include "Resource/Excel/ExcelData.h"

CD2MenuBorder::CD2MenuBorder()
{
	SetTypeID<CD2MenuBorder>();
}

CD2MenuBorder::CD2MenuBorder(const CD2MenuBorder& window) :
	CWidgetWindow(window)
{
}

CD2MenuBorder::~CD2MenuBorder()
{
}

bool CD2MenuBorder::Init()
{
	CWidgetWindow::Init();

	return true;
}

void CD2MenuBorder::Start()
{
	CWidgetWindow::Start();
}

void CD2MenuBorder::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2MenuBorder::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2MenuBorder::Render()
{
	CWidgetWindow::Render();
}

CD2MenuBorder* CD2MenuBorder::Clone()
{
	return new CD2MenuBorder(*this);
}

void CD2MenuBorder::MakeWidget(CExcelData* data)
{
	mBorderImage = CreateWidget<CImage>("MenuBorder");

	std::vector<std::string>* row = data->GetRow("MenuBorder");
	
	// pos, size;
	mBorderImage->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mBorderImage->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
 	mBorderImage->SetTexture("UIBorderRight", TEXT("Dia2UI/800BorderRight.png"));
}
