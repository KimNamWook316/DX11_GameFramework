#include "TileSetEditWindow.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/ResourceManager.h"
#include "IMGUISeperator.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIButton.h"
#include "IMGUIImage.h"
#include "IMGUIInputInt2.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"
#include "IMGUITree.h"
#include "IMGUIPopUpModal.h"

CTileSetEditWindow::CTileSetEditWindow()	:
	mLoadImageButton(nullptr),
	mAtlasImage(nullptr),
	mCropImage(nullptr),
	mAtlasSizeXText(nullptr),
	mAtlasSizeYText(nullptr),
	mTileShapeList(nullptr),
	mSplitSizeInput(nullptr),
	mSplitButton(nullptr),
	mTileNameList(nullptr),
	mOpenCreateTileButton(nullptr),
	mDeleteTileButton(nullptr),
	mTileImage(nullptr),
	mTileNameInput(nullptr),
	mTileShapeWidget(nullptr),
	mRenameButton(nullptr),
	mTileTypeList(nullptr),
	mTileImageStartInput(nullptr),
	mTileImageEndInput(nullptr),
	mSaveCSVButton(nullptr),
	mLoadCSVButton(nullptr),
	mCreateTilePopUp(nullptr),
	mTileSetNameInput(nullptr)
{
}

CTileSetEditWindow::~CTileSetEditWindow()
{
}

bool CTileSetEditWindow::Init()
{
	// Create Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("TileSet Editor");

	mTileSetNameInput = AddWidget<CIMGUITextInput>("TileSet Name");
	mSaveCSVButton = AddWidget<CIMGUIButton>("Save TileSet", 0.f, 0.f);
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	mLoadCSVButton = AddWidget<CIMGUIButton>("Load TileSet", 0.f, 0.f);

	CIMGUITree* tree = AddWidget<CIMGUITree>("Make TileSet");

	mLoadImageButton = tree->AddWidget<CIMGUIButton>("Load Tile Atlas", 0.f, 0.f);

	mAtlasImage = tree->AddWidget<CIMGUIImage>("Tile Atlas");
	line = tree->AddWidget<CIMGUISameLine>("line");
	mCropImage = tree->AddWidget<CIMGUIImage>("Crop Image");

	text = tree->AddWidget<CIMGUIText>("text");
	text->SetText("Atlas Size X : ");
	line = tree->AddWidget<CIMGUISameLine>("line");
	mAtlasSizeXText = tree->AddWidget<CIMGUIText>("AtlasSizeX");
	mAtlasSizeXText->SetText("0");
	line = tree->AddWidget<CIMGUISameLine>("line");
	text = tree->AddWidget<CIMGUIText>("text");
	text->SetText(" Atlas Size Y : ");
	line = tree->AddWidget<CIMGUISameLine>("line");
	mAtlasSizeYText = tree->AddWidget<CIMGUIText>("AtlasSizeY");
	mAtlasSizeYText->SetText("0");

	mTileShapeList = tree->AddWidget<CIMGUIComboBox>("Tile Shape");
	mSplitSizeInput = tree->AddWidget<CIMGUIInputInt2>("Split Size");
	mSplitButton = tree->AddWidget<CIMGUIButton>("Split", 0.f, 0.f);

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Tile Info");
	mTileNameList = AddWidget<CIMGUIComboBox>("Tiles");
	line = AddWidget<CIMGUISameLine>("line");
	mOpenCreateTileButton = AddWidget<CIMGUIButton>("Create Tile", 0.f, 0.f);
	line = AddWidget<CIMGUISameLine>("line");
	mDeleteTileButton = AddWidget<CIMGUIButton>("Delete", 0.f, 0.f);

	mTileImage = AddWidget<CIMGUIImage>("Tile Image");
	mTileNameInput = AddWidget<CIMGUITextInput>("Tile Name");
	mRenameButton = AddWidget<CIMGUIButton>("Rename", 0.f, 0.f);
	mTileShapeWidget = AddWidget<CIMGUITextInput>("Tile Shape");
	mTileTypeList = AddWidget<CIMGUIComboBox>("Tile Type");
	mTileImageStartInput = AddWidget<CIMGUIInputInt2>("Tile Image Start");
	mTileImageEndInput = AddWidget<CIMGUIInputInt2>("Tile Image End");

	// Initial Value	
	mTileSetNameInput->SetHintText("TileSetName");

	mCropImage->SetImageStart(0.f, 0.f);

	for (int i = 0; i < 2; ++i)
	{
		mTileShapeList->AddItem(CUtil::TileShapeToString((eTileShape)i));
	}

	for (int i = 0; i < (int)eTileType::Max; ++i)
	{
		mTileTypeList->AddItem(CUtil::TileTypeToString((eTileType)i));
	}

	mTileShapeWidget->ReadOnly(true);

	// CallBack
	mLoadImageButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickLoadImage);
	mSplitSizeInput->SetCallBack(this, &CTileSetEditWindow::OnChangeSplitSize);
	mSplitButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickSplit);
	mTileNameList->SetSelectCallBack(this, &CTileSetEditWindow::OnSelectTileName);
	mOpenCreateTileButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickOpenCreateTile);
	mDeleteTileButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickDeleteTile);
	mRenameButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickRename);
	mTileTypeList->SetSelectCallBack(this, &CTileSetEditWindow::OnSelectTileType);
	mSaveCSVButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickSaveCSV);
	mLoadCSVButton->SetClickCallBack(this, &CTileSetEditWindow::OnClickLoadCSV);

	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
	resource->CreateTileSet("EditorTileSet");
	mTileSet = resource->FindTileSet("EditorTileSet");
	if (!resource->CreateMaterial<CMaterial>("EditorTileSetMaterial"))
	{
		assert(false);
		return false;
	}
	CMaterial* tileMat = resource->FindMaterial("EditorTileSetMaterial");
	tileMat->SetShader("TileMapShader");
	tileMat->SetRenderState("AlphaBlend");
	mTileSet->SetMaterial(tileMat);

	return true;
}

void CTileSetEditWindow::Clear()
{
	mTileSet->ClearTileInfo();
	mTileSet->ClearTexture();
	mTileNameList->Clear();
	mTileNameInput->SetText("");
	mTileImageStartInput->SetVal(0, 0);
	mTileImageEndInput->SetVal(0, 0);
}

void CTileSetEditWindow::OnClickLoadImage()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("모든 파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetOpenFileName(&openFile) != 0)
	{
		TCHAR fileName[MAX_PATH] = {};

		_wsplitpath_s(filePath, 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);
		char convertFileName[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertFileName, length, 0, 0);

		Clear();

		mTileSet->AddTileTextureFullPath(0, 0, convertFileName, filePath);

		mAtlasImage->SetTexture(mTileSet->GetTexture());

		unsigned int texWidth = mTileSet->GetTexture()->GetWidth();
		unsigned int texHeight = mTileSet->GetTexture()->GetHeight();
		
		char buf[64] = {};
		sprintf_s(buf, "%d", texWidth);
		mAtlasSizeXText->SetText(buf);
		sprintf_s(buf, "%d", texHeight);
		mAtlasSizeYText->SetText(buf);
	}
}

void CTileSetEditWindow::OnChangeSplitSize(int val[2])
{
	if (!mTileSet->GetTexture())
	{
		return;
	}

	if (val[0] < 0 || val[1] < 0)
	{
		return;
	}

	mCropImage->SetTexture(mTileSet->GetTexture());
	mCropImage->SetImageEnd((float)val[0], (float)val[1]);
}

void CTileSetEditWindow::OnClickSplit()
{
	// 예외처리
	if (!mTileSet->GetTexture())
	{
		return;
	}

	if (-1 == mTileShapeList->GetSelectIndex())
	{
		MessageBox(nullptr, TEXT("타일 모양을 선택해야 합니다."), TEXT("Error"), MB_OK);
		return;
	}

	Vector2 splitSize = mSplitSizeInput->GetValue();

	if (splitSize.x <= 0 || splitSize.y <= 0)
	{
		return;
	}

	mTileSet->ClearTileInfo();

	mTileSet->SetTileShape(CUtil::StringToTileShape(mTileShapeList->GetSelectItem()));

	unsigned int atlW = mTileSet->GetTexture()->GetWidth();
	unsigned int atlH = mTileSet->GetTexture()->GetHeight();

	unsigned int countX = atlW / (unsigned int)splitSize.x;
	unsigned int countY = atlH / (unsigned int)splitSize.y;

	for (unsigned int y = 0; y < countY; ++y)
	{
		for (unsigned int x = 0; x < countX; ++x)
		{
			std::string name = std::to_string(y * countX + x);
			Vector2 imageStart = Vector2(x * splitSize.x, y * splitSize.y);
			Vector2 imageEnd = imageStart + splitSize;

			mTileSet->AddTileSetInfo(name, eTileType::Normal, imageStart, imageEnd);
			mTileNameList->AddItem(name);
		}
	}
}

void CTileSetEditWindow::OnSelectTileName(int idx, const char* label)
{
	TileSetInfo* info = mTileSet->FindInfo(label);

	if (!info)
	{
		return;
	}

	mTileImage->SetTexture(mTileSet->GetTexture());
	mTileImage->SetImageStart(info->ImageStart.x, info->ImageStart.y);
	mTileImage->SetImageEnd(info->ImageEnd.x, info->ImageEnd.y);

	mTileImageStartInput->SetVal(info->ImageStart.x, info->ImageStart.y);
	mTileImageEndInput->SetVal(info->ImageEnd.x, info->ImageEnd.y);

	mTileNameInput->SetText(info->Name.c_str());
	mTileTypeList->SetCurrentItem(CUtil::TileTypeToString(info->Type));
	
	mTileShapeWidget->SetText(CUtil::TileShapeToString(info->Shape).c_str());
}

void CTileSetEditWindow::OnClickOpenCreateTile()
{
}

void CTileSetEditWindow::OnClickDeleteTile()
{
	if (mTileNameList->GetSelectIndex() == -1)
	{
		return;
	}

	int idx = mTileNameList->GetSelectIndex();

	if (mTileSet->DeleteInfo(mTileNameList->GetSelectItem()))
	{
		mTileNameList->DeleteItem(idx);
		mTileNameInput->SetText("");
		mTileImageStartInput->SetVal(0, 0);
		mTileImageEndInput->SetVal(0, 0);
	}
}

void CTileSetEditWindow::OnClickRename()
{
	if (-1 == mTileNameList->GetSelectIndex())
	{
		return;
	}

	if (mTileNameInput->IsEmpty())
	{
		return;
	}

	std::string prevName = mTileNameList->GetSelectItem();
	std::string curName = mTileNameInput->GetTextMultiByte();

	TileSetInfo* info = mTileSet->FindInfo(curName);

	// 중복 이름 있는 경우
	if (info)
	{
		MessageBox(nullptr, TEXT("중복된 이름이 있습니다."), TEXT("생성 실패"), MB_OK);
		return;
	}

	mTileSet->RenameTileSetInfo(prevName, curName);

	mTileNameList->DeleteItem(mTileNameList->GetSelectIndex());
	mTileNameList->AddItem(curName);
	mTileNameList->SetCurrentItem(curName);
}

void CTileSetEditWindow::OnSelectTileType(int idx, const char* label)
{
	if (-1 == mTileNameList->GetSelectIndex())
	{
		return;
	}

	TileSetInfo* info = mTileSet->FindInfo(mTileNameList->GetSelectItem());
	info->Type = CUtil::StringToTileType(label);
}

void CTileSetEditWindow::OnClickSaveCSV()
{
	if (mTileSetNameInput->IsEmpty())
	{
		MessageBox(nullptr, TEXT("타일셋 이름을 지정해야 합니다."), TEXT("Failed"), MB_OK);
	}

    TCHAR filePath[MAX_PATH] = {};

    OPENFILENAME openFile = {};

    openFile.lStructSize = sizeof(OPENFILENAME);
    openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    openFile.lpstrFilter = TEXT("csv File\0*.csv\0모든 파일\0*.*");
    openFile.lpstrFile = filePath;
    openFile.nMaxFile = MAX_PATH;
    openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(EXCEL_PATH)->Path;

    if (GetSaveFileName(&openFile) != 0)
    {
        char convertFullPath[MAX_PATH] = {};

        int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		mTileSet->SetName(mTileSetNameInput->GetTextMultiByte());
		if (!mTileSet->SaveCSVFullPath(convertFullPath))
		{
			MessageBox(nullptr, TEXT("타일셋 저장 실패"), TEXT("Failed"), MB_OK);
		}
		MessageBox(nullptr, TEXT("타일셋 저장 성공"), TEXT("Success"), MB_OK);
    }
}

void CTileSetEditWindow::OnClickLoadCSV()
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

		Clear();

		if (!mTileSet->LoadCSVFullPath(convertFullPath))
		{
			MessageBox(nullptr, TEXT("타일셋 로드 실패"), TEXT("Failed"), MB_OK);
		}

		std::vector<std::string> mTileNames;
		mTileSet->GetTileNames(mTileNames);

		TileSetInfo* info = nullptr;
		size_t size = mTileNames.size();
		for (size_t i = 0; i < size; ++i)
		{
			info = mTileSet->FindInfo(mTileNames[i]);
			mTileNameList->AddItem(info->Name);
		}

		mAtlasImage->SetTexture(mTileSet->GetTexture());

		unsigned int texWidth = mTileSet->GetTexture()->GetWidth();
		unsigned int texHeight = mTileSet->GetTexture()->GetHeight();
		
		char buf[64] = {};
		sprintf_s(buf, "%d", (unsigned int)texWidth);
		mAtlasSizeXText->SetText(buf);
		sprintf_s(buf, "%d", (unsigned int)texHeight);
		mAtlasSizeYText->SetText(buf);

		mTileSetNameInput->SetText(mTileSet->GetName().c_str());

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}
