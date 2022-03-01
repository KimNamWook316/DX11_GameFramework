#include "MapGeneratorWidget.h"
#include "Component/ProcedualMapGenerator.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUIInputInt2.h"
#include "IMGUIInputInt.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"

CMapGeneratorWidget::CMapGeneratorWidget()	:
	mTileCountInput(nullptr),
	mPartitionLevelInput(nullptr),
	mNextStepButton(nullptr),
	mCreateMapButton(nullptr)
{
}

CMapGeneratorWidget::~CMapGeneratorWidget()
{
}

bool CMapGeneratorWidget::Init()
{
	// Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("Text");
	text->SetText("Procedual Map Generator");
	mTileCountInput = AddWidget<CIMGUIInputInt2>("Map Size");
	mPartitionLevelInput = AddWidget<CIMGUIInputInt>("Binary Partitioning Level");
	mFindTileComponentButton = AddWidget<CIMGUIButton>("Find Tile Component", 0.f, 0.f);
	mNextStepButton = AddWidget<CIMGUIButton>("Next Step", 0.f, 0.f);
	mCreateMapButton = AddWidget<CIMGUIButton>("Generate Map", 0.f, 0.f);
	CIMGUISeperator* sep = AddWidget<CIMGUISeperator>("sep");

	// CallBack
	mFindTileComponentButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickFindTileComponent);
	mNextStepButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickNextStep);
	mCreateMapButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickCreateMap);

	// Initial Value

	return true;
}

void CMapGeneratorWidget::OnClickNextStep()
{
	Vector2 tileCount = mTileCountInput->GetValue();
	int partitionLevel = mPartitionLevelInput->GetVal();

	if (tileCount.x <= 0 || tileCount.y <= 0)
	{
		MessageBox(nullptr, TEXT("잘못된 맵 크기"), TEXT("Failed"), MB_OK);
		return;
	}
	if (partitionLevel <= 0)
	{
		MessageBox(nullptr, TEXT("잘못된 분할 계수"), TEXT("Failed"), MB_OK);
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetMapCount((int)tileCount.x, (int)tileCount.y);
	gen->SetPartitionLevel(partitionLevel);

	if (gen->DoStep(gen->GetCurrentGenerationStepToDo()))
	{
		int doneStep = gen->GetCurrentGenerationStepDone();
		if (-1 != doneStep)
		{
			switch ((eGenerationStep)(doneStep))
			{
			case eGenerationStep::PartitionSpace:
				MessageBox(nullptr, TEXT("공간 분할 완료"), TEXT("생성 완료"), MB_OK);
				break;
			case eGenerationStep::MakeRoom:
				MessageBox(nullptr, TEXT("방 생성 완료"), TEXT("생성 완료"), MB_OK);
				break;
			case eGenerationStep::ConnectRoom:
				MessageBox(nullptr, TEXT("방 연결 완료"), TEXT("생성 완료"), MB_OK);
				break;
			}
		}
	}
	else
	{
		eGenerationStep step = gen->GetCurrentGenerationStepToDo();
		switch (step)
		{
		case eGenerationStep::PartitionSpace:
			MessageBox(nullptr, TEXT("공간 분할 실패"), TEXT("생성 실패"), MB_OK);
			break;
		case eGenerationStep::MakeRoom:
			MessageBox(nullptr, TEXT("방 생성 실패"), TEXT("생성 실패"), MB_OK);
			break;
		case eGenerationStep::ConnectRoom:
			MessageBox(nullptr, TEXT("방 연결 실패"), TEXT("생성 실패"), MB_OK);
			break;
		}
	}
}

void CMapGeneratorWidget::OnClickCreateMap()
{
	static_cast<CProcedualMapGenerator*>(mComponent)->GenerateMap();
}

void CMapGeneratorWidget::OnClickFindTileComponent()
{
	static_cast<CProcedualMapGenerator*>(mComponent)->FindTileComponent();
}
