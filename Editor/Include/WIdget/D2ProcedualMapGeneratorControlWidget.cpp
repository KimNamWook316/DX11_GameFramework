#include "D2ProcedualMapGeneratorControlWidget.h"
#include "../Diablo2/Component/D2ProcedualMapGenerator.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUIImage.h"
#include "IMGUITextInput.h"
#include "IMGUIInputInt2.h"
#include "IMGUIInputInt.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIDummy.h"
#include "IMGUIComboBox.h"
#include "PathManager.h"
#include "Engine.h"
#include "Resource/TileSet/TileSet.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/SceneComponent.h"
#include "GameObject/GameObject.h"

CD2ProcedualMapGeneratorControlWidget::CD2ProcedualMapGeneratorControlWidget()	:
	mTileCountInput(nullptr),
	mPartitionLevelInput(nullptr),
	mNextStepButton(nullptr),
	mCreateMapButton(nullptr),
	mTileSetName(nullptr),
	mLoadTileSetButton(nullptr),
	mTileSetImage(nullptr),
	mWallTileSetName(nullptr),
	mLoadWallTileSetButton(nullptr),
	mWallTileSetImage(nullptr),
	mTileSizeInput(nullptr),
	mTileShapeComboBox(nullptr),
	mRoomSizeMinInput(nullptr)
{
}

CD2ProcedualMapGeneratorControlWidget::~CD2ProcedualMapGeneratorControlWidget()
{
}

bool CD2ProcedualMapGeneratorControlWidget::Init()
{
	// Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("Text");
	text->SetText("Procedual Map Generator");
	mTileSizeInput = AddWidget<CIMGUIInputInt2>("Tile Size");
	mTileShapeComboBox = AddWidget<CIMGUIComboBox>("Tile Shape");
	mRoomSizeMinInput = AddWidget<CIMGUIInputInt>("Min Room Size");
	mTileCountInput = AddWidget<CIMGUIInputInt2>("Map Size");
	mPartitionLevelInput = AddWidget<CIMGUIInputInt>("Binary Partitioning Level");

	CIMGUIDummy* dummy = AddWidget<CIMGUIDummy>("dummy", 10.f, 10.f);
	text = AddWidget<CIMGUIText>("Text");
	text->SetText("TileSet");
	mTileSetName = AddWidget<CIMGUITextInput>("TileSet");
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	mLoadTileSetButton = AddWidget<CIMGUIButton>("Load TileSet", 0.f, 0.f);
	text = AddWidget<CIMGUIText>("Text");
	text->SetText("TileSet Preview");
	mTileSetImage = AddWidget<CIMGUIImage>("TileSetImage");

	dummy = AddWidget<CIMGUIDummy>("dummy", 10.f, 10.f);
	text = AddWidget<CIMGUIText>("Text");
	text->SetText("Wall TileSet");
	mWallTileSetName = AddWidget<CIMGUITextInput>("WallTileSet");
	line = AddWidget<CIMGUISameLine>("line");
	mLoadWallTileSetButton = AddWidget <CIMGUIButton>("Load WallTileSet", 0.f, 0.f);
	text = AddWidget<CIMGUIText>("Text");
	text->SetText("WallTileSet Preview");
	mWallTileSetImage = AddWidget<CIMGUIImage>("WallTileSetImage");

	mNextStepButton = AddWidget<CIMGUIButton>("Next Step", 0.f, 0.f);
	mCreateMapButton = AddWidget<CIMGUIButton>("Generate Map", 0.f, 0.f);
	CIMGUISeperator* sep = AddWidget<CIMGUISeperator>("sep");

	// CallBack
	mNextStepButton->SetClickCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnClickNextStep);
	mCreateMapButton->SetClickCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnClickCreateMap);
	mLoadTileSetButton->SetClickCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnClickLoadTileSet);
	mLoadWallTileSetButton->SetClickCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnClickLoadWallTileSet);
	mTileSizeInput->SetCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnChangeTileSize);
	mTileShapeComboBox->SetSelectCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnChangeTileShape);
	mRoomSizeMinInput->SetCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnChangeRoomSizeMin);
	mTileCountInput->SetCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnChangeTileCount);
	mPartitionLevelInput->SetCallBack(this, &CD2ProcedualMapGeneratorControlWidget::OnChangePartitionLevel);

	// Initial Value
	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
	mTileSetName->ReadOnly(true);
	mTileSetName->SetHintText("TileSetName");
	mWallTileSetName->ReadOnly(true);
	mWallTileSetName->SetHintText("WallTileSetName");
	mTileSizeInput->SetVal(gen->GetTileSize().x, gen->GetTileSize().y);
	for (int i = 0; i < 2; ++i)
	{
		mTileShapeComboBox->AddItem(CUtil::TileShapeToString((eTileShape)i));
	}
	mTileShapeComboBox->SetCurrentItem(CUtil::TileShapeToString(gen->GetTileShape()));
	mRoomSizeMinInput->SetVal(gen->GetRoomSizeMin());
	mTileCountInput->SetVal(gen->GetMapCountX(), gen->GetMapCountY());
	mPartitionLevelInput->SetVal(gen->GetPartitionLevel());

	CTileSet* tileSet = gen->GetTileSet();
	if (tileSet)
	{
		mTileSetName->SetText(tileSet->GetName().c_str());

		unsigned int texWidth = tileSet->GetTexture()->GetWidth();
		unsigned int texHeight = tileSet->GetTexture()->GetHeight();

		mTileSetImage->SetTexture(tileSet->GetTexture());
		mTileSetImage->SetImageStart(0.f, 0.f);
		mTileSetImage->SetImageEnd(texWidth, texHeight);
	}

	tileSet = gen->GetWallTileSet();
	if (tileSet)
	{
		mWallTileSetName->SetText(tileSet->GetName().c_str());

		unsigned int texWidth = tileSet->GetTexture()->GetWidth();
		unsigned int texHeight = tileSet->GetTexture()->GetHeight();

		mWallTileSetImage->SetTexture(tileSet->GetTexture());
		mWallTileSetImage->SetImageStart(0.f, 0.f);
		mWallTileSetImage->SetImageEnd(texWidth, texHeight);
	}

	return true;
}

// 맵 생성 시각화, 디버깅용 함수
void CD2ProcedualMapGeneratorControlWidget::OnClickNextStep()
{
	Vector2 tileCount = mTileCountInput->GetValue();
	int partitionLevel = mPartitionLevelInput->GetVal();

	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());

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
	if (!gen->GetTileSet())
	{
		MessageBox(nullptr, TEXT("타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}
	if (!gen->GetWallTileSet())
	{
		MessageBox(nullptr, TEXT("벽 타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}

	gen->SetMapCount((int)tileCount.x, (int)tileCount.y);
	gen->SetPartitionLevel(partitionLevel);

	bool bSuccess = false;

	eD2ProcedualGenerationStep curStep = gen->GetCurGenerationStep();
	switch (curStep)
	{
	case eD2ProcedualGenerationStep::PartitionSpace:
		bSuccess = gen->PartitionSpaceDebug();
		break;
	case eD2ProcedualGenerationStep::MakeRoom:
		bSuccess = gen->MakeRoomDebug();
		break;
	case eD2ProcedualGenerationStep::ConnectRoom:
		bSuccess = gen->ConnectRoomDebug();
		break;
	case eD2ProcedualGenerationStep::CreatePathFindInfo:
		bSuccess = gen->CreatePathFindInfoDebug();
		break;
	case eD2ProcedualGenerationStep::GenerateWall:
		bSuccess = gen->GenerateWallDebug();
		break;
	case eD2ProcedualGenerationStep::Spawn:
		bSuccess = gen->SpawnDebug();
		break;
	}

	if (!bSuccess)
	{
		MessageBox(nullptr, TEXT("생성 실패"), TEXT("Failed"), MB_OK);
	}
}

void CD2ProcedualMapGeneratorControlWidget::OnClickCreateMap()
{
	Vector2 tileCount = mTileCountInput->GetValue();
	int partitionLevel = mPartitionLevelInput->GetVal();

	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());

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
	if (!gen->GetTileSet())
	{
		MessageBox(nullptr, TEXT("타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}
	if (!gen->GetWallTileSet())
	{
		MessageBox(nullptr, TEXT("벽 타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}

	gen->SetMapCount((int)tileCount.x, (int)tileCount.y);
	gen->SetPartitionLevel(partitionLevel);

	static_cast<CD2ProcedualMapGenerator*>(mComponent.Get())->GenerateMap();
}

void CD2ProcedualMapGeneratorControlWidget::OnClickLoadTileSet()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("csv File\0*.csv\0모든 파일\0*.*");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(EXCEL_PATH)->Path;

	if (GetOpenFileName(&openFile) != 0)
	{
        char convertFullPath[MAX_PATH] = {};
        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		std::string outName;
		mComponent->GetGameObject()->GetScene()->GetResource()->LoadTileSetFullPath(outName, convertFullPath);
		CTileSet* tileSet = mComponent->GetGameObject()->GetScene()->GetResource()->FindTileSet(outName);

		mTileSetName->SetText(outName.c_str());

		unsigned int texWidth = tileSet->GetTexture()->GetWidth();
		unsigned int texHeight = tileSet->GetTexture()->GetHeight();

		mTileSetImage->SetTexture(tileSet->GetTexture());
		mTileSetImage->SetImageStart(0.f, 0.f);
		mTileSetImage->SetImageEnd(texWidth, texHeight);

		CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
		gen->SetTileSet(tileSet);

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}

void CD2ProcedualMapGeneratorControlWidget::OnClickLoadWallTileSet()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("csv File\0*.csv\0모든 파일\0*.*");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(EXCEL_PATH)->Path;

	if (GetOpenFileName(&openFile) != 0)
	{
        char convertFullPath[MAX_PATH] = {};
        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		std::string outName;
		mComponent->GetGameObject()->GetScene()->GetResource()->LoadTileSetFullPath(outName, convertFullPath);
		CTileSet* wallTileSet = mComponent->GetGameObject()->GetScene()->GetResource()->FindTileSet(outName);

		mWallTileSetName->SetText(outName.c_str());

		CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
		gen->SetWallTileSet(wallTileSet);

		unsigned int texWidth = wallTileSet->GetTexture()->GetWidth();
		unsigned int texHeight = wallTileSet->GetTexture()->GetHeight();

		mWallTileSetImage->SetTexture(wallTileSet->GetTexture());
		mWallTileSetImage->SetImageStart(0.f, 0.f);
		mWallTileSetImage->SetImageEnd(texWidth, texHeight);

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}

void CD2ProcedualMapGeneratorControlWidget::OnChangeTileSize(int val[2])
{
	if (val[0] < 0 || val[1] < 0)
	{
		return;
	}

	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
	gen->SetTileSize(Vector3(val[0], val[2], 0.f));
}

void CD2ProcedualMapGeneratorControlWidget::OnChangeTileShape(int idx, const char* label)
{
	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
	gen->SetTileShape(CUtil::StringToTileShape(label));
}

void CD2ProcedualMapGeneratorControlWidget::OnChangeRoomSizeMin(int val)
{
	if (val <= 5)
	{
		return;
	}

	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
	gen->SetRoomSizeMin(val);
}

void CD2ProcedualMapGeneratorControlWidget::OnChangeTileCount(int val[2])
{
	if (val[0] < 14 || val[1] < 14)
	{
		return;
	}

	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
	gen->SetMapCount(val[0], val[1]);
}

void CD2ProcedualMapGeneratorControlWidget::OnChangePartitionLevel(int val)
{
	if (val <= 0)
	{
		return;
	}

	CD2ProcedualMapGenerator* gen = static_cast<CD2ProcedualMapGenerator*>(mComponent.Get());
	gen->SetPartitionLevel(val);
}
