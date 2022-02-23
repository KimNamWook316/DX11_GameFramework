#include "TileMapControlWidget.h"
#include "../Window/TileSetEditWindow.h"
#include "../EditorManager.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Component/TileMapComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "PathManager.h"
#include "IMGUIWindow.h"
#include "IMGUIImage.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIInputInt2.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUITree.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"

CTileMapControlWidget::CTileMapControlWidget()	:
	mTileImageWidget(nullptr),
	mTileShapeWidget(nullptr),
	mTileSetPreviewImage(nullptr),
	mTileCountWidget(nullptr),
	mTileSizeWidget(nullptr),
	mCreateMapButton(nullptr),
	mTileTypeWidget(nullptr),
	mDefaultFrameButon(nullptr),
	mOpenTileSetEditButton(nullptr),
	mbIsTileMapCreate(false),
	mSelectInfo(nullptr)
{
}

CTileMapControlWidget::~CTileMapControlWidget()
{
}

bool CTileMapControlWidget::Init()
{
	// Create Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("TileMap Component");

	mOpenTileSetEditButton = AddWidget<CIMGUIButton>("Edit TileSet", 0.f, 0.f);

	CIMGUITree* tree = AddWidget<CIMGUITree>("Create TileMap");
	mTileSetNameWidget = tree->AddWidget<CIMGUITextInput>("TileSet Name");
	CIMGUISameLine* line = tree->AddWidget<CIMGUISameLine>("line");
	mTileSetLoadButton = tree->AddWidget<CIMGUIButton>("Load", 0.f, 0.f);
	mTileSetPreviewImage = tree->AddWidget<CIMGUIImage>("Preview");
	mTileCountWidget = tree->AddWidget<CIMGUIInputInt2>("Tile Count");
	mTileSizeWidget = tree->AddWidget<CIMGUIInputInt2>("Tile Size");
	mCreateMapButton = tree->AddWidget<CIMGUIButton>("Create Map", 0.f, 0.f);

	tree = AddWidget<CIMGUITree>("TileMap Info");
	mTileSetTileList = tree->AddWidget<CIMGUIComboBox>("Tiles");
	line = tree->AddWidget<CIMGUISameLine>("line");
	mDefaultFrameButon = tree->AddWidget<CIMGUIButton>("Set Default", 0.f, 0.f);
	mTileImageWidget = tree->AddWidget<CIMGUIImage>("Tile View");
	mTileShapeWidget = tree->AddWidget<CIMGUITextInput>("Shape");
	mTileTypeWidget = tree->AddWidget<CIMGUITextInput>("Type");

	AddWidget<CIMGUISeperator>("sperator");

	// Initial Value
	mTileSetNameWidget->ReadOnly(true);
	mTileSetNameWidget->SetHintText("TileSet Name");
	mTileShapeWidget->ReadOnly(true);
	mTileShapeWidget->SetHintText("Tile Shape");
	mTileTypeWidget->ReadOnly(true);
	mTileTypeWidget->SetHintText("Tile Type");

	// CallBack
	mOpenTileSetEditButton->SetClickCallBack(this, &CTileMapControlWidget::OnClickOpenTileSetEdit);
	mTileSetLoadButton->SetClickCallBack(this, &CTileMapControlWidget::OnClickLoadTileSet);
	mTileSetTileList->SetSelectCallBack(this, &CTileMapControlWidget::OnSelectTileList);
	mCreateMapButton->SetClickCallBack(this, &CTileMapControlWidget::OnClickCreateMap);
	mDefaultFrameButon->SetClickCallBack(this, &CTileMapControlWidget::OnClickDefaultFrame);

	return true;
}

void CTileMapControlWidget::Render()
{
	CComponentControlWidget::Render();

	if (eEditMode::TileMap == CEditorManager::GetInst()->GetEditMode() && mbIsTileMapCreate)
	{
		if (CEditorManager::GetInst()->GetLButtonPush())
		{
			CTileMapComponent* tileMap = static_cast<CTileMapComponent*>(mComponent);
			tileMap->EnableEditMode(true);

			Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

			CTile* tile = tileMap->GetTile(Vector3(mouseWorldPos.x, mouseWorldPos.y, 0.f));

			if (tile)
			{
				if (mSelectInfo)
				{
					tile->SetFrameStart(mSelectInfo->ImageStart);
					tile->SetFrameEnd(mSelectInfo->ImageEnd);
					tile->SetTileType(mSelectInfo->Type);
				}
			}
		}
	}
}

void CTileMapControlWidget::OnClickCreateMap()
{
	if (!static_cast<CTileMapComponent*>(mComponent)->GetTileSet())
	{
		MessageBox(nullptr, TEXT("타일셋을 설정해야 합니다.."), TEXT("Creation Failed"), MB_OK);
		return;
	}

	int countX = mTileCountWidget->GetX();
	int countY = mTileCountWidget->GetY();
	int sizeX = mTileSizeWidget->GetX();
	int sizeY = mTileSizeWidget->GetY();

	if (0 >= countX || 0 >= countY || 0 >= sizeX || 0 >= sizeY)
	{
		MessageBox(nullptr, TEXT("타일 개수와 크기가 올바르지 않습니다."), TEXT("Creation Failed"), MB_OK);
		return;
	}

	if (!static_cast<CTileMapComponent*>(mComponent)->CreateTile(countX, countY, Vector2(sizeX, sizeY)))
	{
		MessageBox(nullptr, TEXT("타일맵 생성 실패."), TEXT("Creation Failed"), MB_OK);
		return;
	}

	mbIsTileMapCreate = true;
}

void CTileMapControlWidget::OnSelectTileList(int idx, const char* label)
{
	CTileMapComponent* comp =  static_cast<CTileMapComponent*>(mComponent);

	TileSetInfo* info = comp->GetTileSet()->FindInfo(label);
	
	mSelectInfo = info;

	mTileImageWidget->SetTexture(comp->GetTileSet()->GetTexture());
	mTileImageWidget->SetImageStart(info->ImageStart.x, info->ImageStart.y);
	mTileImageWidget->SetImageEnd(info->ImageEnd.x, info->ImageEnd.y);

	mTileShapeWidget->SetText(CUtil::TileShapeToString(info->Shape).c_str());
	mTileTypeWidget->SetText(CUtil::TileTypeToString(info->Type).c_str());
}

void CTileMapControlWidget::OnClickDefaultFrame()
{
	CTileMapComponent* comp =  static_cast<CTileMapComponent*>(mComponent);

	if (-1 == mTileSetTileList->GetSelectIndex())
	{
		return;
	}

	comp->SetTileDefaultInfo(mTileSetTileList->GetSelectItem());
}

void CTileMapControlWidget::clearWidget()
{
	mTileSetNameWidget->SetText("");
	mTileSetNameWidget->SetHintText("TileSet Name");
	mTileSetTileList->Clear();
	mTileShapeWidget->SetText("");
	mTileTypeWidget->SetText("");
}

void CTileMapControlWidget::OnClickOpenTileSetEdit()
{
	CIMGUIManager::GetInst()->AddWindow<CTileSetEditWindow>("TileSet Editor");
}

void CTileMapControlWidget::OnClickLoadTileSet()
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
		// fullPath
		char convertFullPath[MAX_PATH] = {};
        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		// fileName
		TCHAR fileName[MAX_PATH] = {};
		_wsplitpath_s(filePath, 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);
		char convertFileName[MAX_PATH] = {};
		length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertFileName, length, 0, 0);

		CSceneResource* resource = mComponent->GetGameObject()->GetScene()->GetResource();
		CTileMapComponent* tileComp = static_cast<CTileMapComponent*>(mComponent);

		// 타일셋 로드
		std::string tileSetName;
		if (!resource->LoadTileSetFullPath(tileSetName, convertFullPath))
		{
			MessageBox(nullptr, TEXT("타일셋 로드 실패"), TEXT("Error"), MB_OK);
			return;
		}

		// 타일셋 설정
		CTileSet* tileSet = resource->FindTileSet(tileSetName);
		tileComp->SetTileSet(tileSet);

		// 위젯 업데이트
		clearWidget();

		mTileSetNameWidget->SetText(tileSetName.c_str());
		mTileSetPreviewImage->SetTexture(tileSet->GetTexture());

		std::vector<std::string> tileNames;
		tileSet->GetTileNames(tileNames);
		
		size_t size = tileNames.size();
		for (size_t i = 0; i < size; ++i)
		{
			mTileSetTileList->AddItem(tileNames[i]);
		}
	}
}
