#include "MapGeneratorWidget.h"
#include "Component/ProcedualMapGenerator.h"
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

CMapGeneratorWidget::CMapGeneratorWidget()	:
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
	mRoomSizeMinInput(nullptr),
	mbGenerationStep{}
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
	mNextStepButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickNextStep);
	mCreateMapButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickCreateMap);
	mLoadTileSetButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickLoadTileSet);
	mLoadWallTileSetButton->SetClickCallBack(this, &CMapGeneratorWidget::OnClickLoadWallTileSet);
	mTileSizeInput->SetCallBack(this, &CMapGeneratorWidget::OnChangeTileSize);
	mTileShapeComboBox->SetSelectCallBack(this, &CMapGeneratorWidget::OnChangeTileShape);
	mRoomSizeMinInput->SetCallBack(this, &CMapGeneratorWidget::OnChangeRoomSizeMin);
	mTileCountInput->SetCallBack(this, &CMapGeneratorWidget::OnChangeTileCount);
	mPartitionLevelInput->SetCallBack(this, &CMapGeneratorWidget::OnChangePartitionLevel);

	// Initial Value
	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
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

	for (int i = 0; i < (int)eGenerationStep::Max; ++i)
	{
		mbGenerationStep[i] = false;
	}

	return true;
}

void CMapGeneratorWidget::OnClickNextStep()
{
	Vector2 tileCount = mTileCountInput->GetValue();
	int partitionLevel = mPartitionLevelInput->GetVal();

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);

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
	
	eGenerationStep curStep = eGenerationStep::Max;
	for (int i = 0; i < (int)eGenerationStep::Max; ++i)
	{
		if (!mbGenerationStep[i])
		{
			curStep = (eGenerationStep)i;
			break;
		}
	}

	bool success = false;
	switch (curStep)
	{
	case SplitArea:
		success = gen->PartitionSpace();
		break;
	case MakeRoom:
		success = gen->MakeRoom();
		break;
	case MakePath:
		success = gen->ConnectRoom();
		break;
	case MakeWall:
		success = gen->GenerateWall();
		break;
	}

	if (success)
	{
		mbGenerationStep[(int)curStep] = true;
	}
}

void CMapGeneratorWidget::OnClickCreateMap()
{
	Vector2 tileCount = mTileCountInput->GetValue();
	int partitionLevel = mPartitionLevelInput->GetVal();

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);

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

	static_cast<CProcedualMapGenerator*>(mComponent)->GenerateMap();
}

void CMapGeneratorWidget::OnClickLoadTileSet()
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

		CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
		gen->SetTileSet(tileSet);

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}

void CMapGeneratorWidget::OnClickLoadWallTileSet()
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

		CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
		gen->SetWallTileSet(wallTileSet);

		unsigned int texWidth = wallTileSet->GetTexture()->GetWidth();
		unsigned int texHeight = wallTileSet->GetTexture()->GetHeight();

		mWallTileSetImage->SetTexture(wallTileSet->GetTexture());
		mWallTileSetImage->SetImageStart(0.f, 0.f);
		mWallTileSetImage->SetImageEnd(texWidth, texHeight);

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}

void CMapGeneratorWidget::OnChangeTileSize(int val[2])
{
	if (val[0] < 0 || val[1] < 0)
	{
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetTileSize(Vector3(val[0], val[2], 0.f));
}

void CMapGeneratorWidget::OnChangeTileShape(int idx, const char* label)
{
	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetTileShape(CUtil::StringToTileShape(label));
}

void CMapGeneratorWidget::OnChangeRoomSizeMin(int val)
{
	if (val <= 5)
	{
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetRoomSizeMin(val);
}

void CMapGeneratorWidget::OnChangeTileCount(int val[2])
{
	if (val[0] < 14 || val[1] < 14)
	{
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetMapCount(val[0], val[1]);
}

void CMapGeneratorWidget::OnChangePartitionLevel(int val)
{
	if (val <= 0)
	{
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetPartitionLevel(val);
}
