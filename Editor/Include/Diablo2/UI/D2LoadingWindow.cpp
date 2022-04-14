#include "D2LoadingWindow.h"
#include "Resource/Excel/ExcelData.h"

CD2LoadingWindow::CD2LoadingWindow()
{
}

CD2LoadingWindow::CD2LoadingWindow(const CD2LoadingWindow& window)	:
	CWidgetWindow(window)
{
}

CD2LoadingWindow::~CD2LoadingWindow()
{
}

bool CD2LoadingWindow::Init()
{
	CWidgetWindow::Init();
	return true;
}

void CD2LoadingWindow::Start()
{
	CWidgetWindow::Start();
}

void CD2LoadingWindow::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CD2LoadingWindow::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CD2LoadingWindow::Render()
{
	CWidgetWindow::Render();
}

CD2LoadingWindow* CD2LoadingWindow::Clone()
{
	return new CD2LoadingWindow(*this);
}

void CD2LoadingWindow::SetLoadingPercent(const float percent)
{
	int frame = percent * mLoadingImage->GetAnimationFrameCount();
	mLoadingImage->SetCurrentFrame(frame);
}

void CD2LoadingWindow::MakeWidget(CExcelData* data)
{
	// Craete
	mLoadingImage = CreateWidget<CImage>("LoadingImage");

	// Pos, Size
	std::vector<std::string>* row = data->GetRow("LoadingScreen");
	mLoadingImage->SetSize(std::stof((*row)[1]), std::stof((*row)[2]));
	mLoadingImage->SetPos(std::stof((*row)[3]), std::stof((*row)[4]));

	// Init
	mLoadingImage->SetTexture("LoadingScreen", TEXT("Dia2UI/LoadingScreen.png"));
	
	for (int i = 0; i < 4; ++i)
	{
		mLoadingImage->AddFrameData(Vector2(i * 800.f, 0.f), Vector2(800.f, 600.f));
	}

	mLoadingImage->SetAnimationPlayScale(0.f);
}
