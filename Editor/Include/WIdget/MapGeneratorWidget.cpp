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
	mWallTileSetImage(nullptr)
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

	mTileSetName = AddWidget<CIMGUITextInput>("TileSet");
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	mLoadTileSetButton = AddWidget<CIMGUIButton>("Load TileSet", 0.f, 0.f);
	text = AddWidget<CIMGUIText>("Text");
	text->SetText("TileSet Preview");
	mTileSetImage = AddWidget<CIMGUIImage>("TileSetImage");

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

	// Initial Value
	mTileSetName->ReadOnly(true);
	mTileSetName->SetHintText("TileSetName");
	mWallTileSetName->ReadOnly(true);
	mWallTileSetName->SetHintText("WallTileSetName");

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
	if (!mTileSet)
	{
		MessageBox(nullptr, TEXT("타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}
	if (!mWallTileSet)
	{
		MessageBox(nullptr, TEXT("벽 타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
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
		gen->SetTileSet(mTileSet);
		success = gen->MakeRoom();
		break;
	case MakePath:
		success = gen->ConnectRoom();
		break;
	case MakeWall:
		gen->SetWallTileSet(mWallTileSet);
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
	if (!mTileSet)
	{
		MessageBox(nullptr, TEXT("타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}
	if (!mWallTileSet)
	{
		MessageBox(nullptr, TEXT("벽 타일셋 없음"), TEXT("Failed"), MB_OK);
		return;
	}

	CProcedualMapGenerator* gen = static_cast<CProcedualMapGenerator*>(mComponent);
	gen->SetTileSet(mTileSet);
	gen->SetWallTileSet(mWallTileSet);
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
		mTileSet = mComponent->GetGameObject()->GetScene()->GetResource()->FindTileSet(outName);

		mTileSetName->SetText(outName.c_str());

		unsigned int texWidth = mTileSet->GetTexture()->GetWidth();
		unsigned int texHeight = mTileSet->GetTexture()->GetHeight();

		mTileSetImage->SetTexture(mTileSet->GetTexture());
		mTileSetImage->SetImageStart(0.f, 0.f);
		mTileSetImage->SetImageEnd(texWidth, texHeight);

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
		mWallTileSet = mComponent->GetGameObject()->GetScene()->GetResource()->FindTileSet(outName);

		mWallTileSetName->SetText(outName.c_str());

		unsigned int texWidth = mWallTileSet->GetTexture()->GetWidth();
		unsigned int texHeight = mWallTileSet->GetTexture()->GetHeight();

		mWallTileSetImage->SetTexture(mWallTileSet->GetTexture());
		mWallTileSetImage->SetImageStart(0.f, 0.f);
		mWallTileSetImage->SetImageEnd(texWidth, texHeight);

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}
