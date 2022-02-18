#include "TileSetEditWindow.h"
#include "Engine.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "IMGUISeperator.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUIImage.h"
#include "IMGUIInputInt2.h"
#include "IMGUIComboBox.h"
#include "IMGUITextInput.h"
#include "IMGUIPopUpModal.h"

CTileSetEditWindow::CTileSetEditWindow()	:
	mLoadImageButton(nullptr),
	mAtlasImage(nullptr),
	mCropImage(nullptr),
	mAtlasSizeXText(nullptr),
	mAtlasSizeYText(nullptr),
	mSplitSizeInput(nullptr),
	mSplitButton(nullptr),
	mTileNameList(nullptr),
	mOpenCreateTileButton(nullptr),
	mDeleteTileButton(nullptr),
	mTileImage(nullptr),
	mTileNameInput(nullptr),
	mRenameButton(nullptr),
	mTileTypeList(nullptr),
	mTileImageStartInput(nullptr),
	mTileImageEndInput(nullptr),
	mSaveCSVButton(nullptr),
	mLoadCSVButton(nullptr),
	mCreateTilePopUp(nullptr),
	mTextureFullPath{},
	mCSV(nullptr)
{
}

CTileSetEditWindow::~CTileSetEditWindow()
{
	auto iter = mMapTileSet.begin();
	auto iterEnd = mMapTileSet.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mMapTileSet.clear();
}

bool CTileSetEditWindow::Init()
{
	// Create Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("TileSet Editor");

	mLoadImageButton = AddWidget<CIMGUIButton>("Load Tile Atlas", 0.f, 0.f);

	mAtlasImage = AddWidget<CIMGUIImage>("Tile Atlas");
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	mCropImage = AddWidget<CIMGUIImage>("Crop Image");

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Atlas Size X : ");
	line = AddWidget<CIMGUISameLine>("line");
	mAtlasSizeXText = AddWidget<CIMGUIText>("AtlasSizeX");
	mAtlasSizeXText->SetText("0");
	line = AddWidget<CIMGUISameLine>("line");
	text = AddWidget<CIMGUIText>("text");
	text->SetText(" Atlas Size Y : ");
	line = AddWidget<CIMGUISameLine>("line");
	mAtlasSizeYText = AddWidget<CIMGUIText>("AtlasSizeY");
	mAtlasSizeYText->SetText("0");

	mSplitSizeInput = AddWidget<CIMGUIInputInt2>("Split Size");
	mSplitButton = AddWidget<CIMGUIButton>("Split", 0.f, 0.f);

	mTileNameList = AddWidget<CIMGUIComboBox>("Tiles");
	line = AddWidget<CIMGUISameLine>("line");
	mOpenCreateTileButton = AddWidget<CIMGUIButton>("Create Tile", 0.f, 0.f);
	line = AddWidget<CIMGUISameLine>("line");
	mDeleteTileButton = AddWidget<CIMGUIButton>("Delete", 0.f, 0.f);

	mTileImage = AddWidget<CIMGUIImage>("Tile Image");
	mTileNameInput = AddWidget<CIMGUITextInput>("Tile Name");
	mRenameButton = AddWidget<CIMGUIButton>("Rename", 0.f, 0.f);
	mTileTypeList = AddWidget<CIMGUIComboBox>("Tile Type");
	mTileImageStartInput = AddWidget<CIMGUIInputInt2>("Tile Image Start");
	mTileImageEndInput = AddWidget<CIMGUIInputInt2>("Tile Image End");

	mSaveCSVButton = AddWidget<CIMGUIButton>("Save TileSet", 0.f, 0.f);
	mLoadCSVButton = AddWidget<CIMGUIButton>("Load TileSet", 0.f, 0.f);

	// Initial Value	
	mCropImage->SetImageStart(0.f, 0.f);

	for (int i = 0; i < (int)eTileType::Max; ++i)
	{
		mTileTypeList->AddItem(CUtil::TileTypeToString((eTileType)i));
	}

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

	// CSVData
	CResourceManager::GetInst()->CreateCSV("TileSet");
	mCSV = CResourceManager::GetInst()->FindCSV("TileSet");
	mCSV->AddLabel("TextureFullPath");
	mCSV->AddLabel("TileType");
	mCSV->AddLabel("StartUVX");
	mCSV->AddLabel("StartUVY");
	mCSV->AddLabel("EndUVX");
	mCSV->AddLabel("EndUVY");

	return true;
}

void CTileSetEditWindow::Clear()
{
	auto iter = mMapTileSet.begin();
	auto iterEnd = mMapTileSet.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	mMapTileSet.clear();

	mTileNameList->Clear();
	mTileNameInput->SetText("");
	mTileImageStartInput->SetVal(0, 0);
	mTileImageEndInput->SetVal(0, 0);

	mCSV->ClearData();
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

		if (!CResourceManager::GetInst()->LoadTextureFullPath(convertFileName, filePath))
		{
			MessageBox(nullptr, TEXT("Failed To Load Texture"), TEXT("Error"), MB_OK);
		}
		
		mTextureAtlas = CResourceManager::GetInst()->FindTexture(convertFileName);
		mAtlasImage->SetTexture(mTextureAtlas);

		char buf[64] = {};
		sprintf_s(buf, "%d", mTextureAtlas->GetWidth());
		mAtlasSizeXText->SetText(buf);
		sprintf_s(buf, "%d", mTextureAtlas->GetHeight());
		mAtlasSizeYText->SetText(buf);

		// Texture Path 저장
		char convertFullPath[MAX_PATH] = {};
		length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, nullptr, nullptr);
		std::string fullPath = convertFullPath;
		mCSV->SetData("TextureInfo", "TextureFullPath", fullPath);
	}
}

void CTileSetEditWindow::OnChangeSplitSize(int val[2])
{
	if (!mTextureAtlas)
	{
		return;
	}

	if (val[0] < 0 || val[1] < 0)
	{
		return;
	}

	mCropImage->SetTexture(mTextureAtlas);
	mCropImage->SetImageEnd((float)val[0], (float)val[1]);
}

void CTileSetEditWindow::OnClickSplit()
{
	// 예외처리
	if (!mTextureAtlas)
	{
		return;
	}

	Vector2 splitSize = mSplitSizeInput->GetValue();

	if (splitSize.x <= 0 || splitSize.y <= 0)
	{
		return;
	}

	if (!mMapTileSet.empty())
	{
		Clear();
	}

	unsigned int atlW = mTextureAtlas->GetWidth();
	unsigned int atlH = mTextureAtlas->GetHeight();

	unsigned int countX = atlW / (unsigned int)splitSize.x;
	unsigned int countY = atlH / (unsigned int)splitSize.y;

	TileSetInfo* info = nullptr;

	for (unsigned int y = 0; y < countY; ++y)
	{
		for (unsigned int x = 0; x < countX; ++x)
		{
			info = new TileSetInfo;
			info->Name = std::to_string(y * countX + x);
			info->Type = eTileType::Normal;
			info->ImageStart = Vector2(x * splitSize.x, y * splitSize.y);
			info->ImageEnd = info->ImageStart + splitSize;

			mMapTileSet.insert(std::make_pair(info->Name, info));

			mCSV->AddRow(info->Name);
			std::vector<std::string> rowBuf;
			rowBuf.push_back("");									// TexturePath
			rowBuf.push_back(CUtil::TileTypeToString(info->Type));	// Type
			rowBuf.push_back(std::to_string(info->ImageStart.x));	// StartUVX
			rowBuf.push_back(std::to_string(info->ImageStart.y));	// StartUVY
			rowBuf.push_back(std::to_string(info->ImageEnd.x));		// EndUVX
			rowBuf.push_back(std::to_string(info->ImageEnd.y));		// EndUVY
			mCSV->SetData(info->Name, rowBuf);

			mTileNameList->AddItem(info->Name);
		}
	}
}

void CTileSetEditWindow::OnSelectTileName(int idx, const char* label)
{
	TileSetInfo* info = FindInfo(label);

	if (!info)
	{
		return;
	}

	mTileImage->SetTexture(mTextureAtlas);
	mTileImage->SetImageStart(info->ImageStart.x, info->ImageStart.y);
	mTileImage->SetImageEnd(info->ImageEnd.x, info->ImageEnd.y);

	mTileImageStartInput->SetVal(info->ImageStart.x, info->ImageStart.y);
	mTileImageEndInput->SetVal(info->ImageEnd.x, info->ImageEnd.y);

	mTileNameInput->SetText(info->Name.c_str());
	mTileTypeList->SetCurrentItem(CUtil::TileTypeToString(info->Type));
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
	if (DeleteInfo(mTileNameList->GetSelectItem()))
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

	TileSetInfo* info = FindInfo(curName);

	// 중복 이름 있는 경우
	if (info)
	{
		MessageBox(nullptr, TEXT("중복된 이름이 있습니다."), TEXT("생성 실패"), MB_OK);
		return;
	}

	info = FindInfo(prevName);

	auto iter = mMapTileSet.begin();
	auto iterEnd = mMapTileSet.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->first == info->Name)
		{
			mMapTileSet.erase(iter);
			break;
		}
	}
	
	info->Name = curName;
	mMapTileSet.insert(std::make_pair(curName, info));

	mCSV->RenameRow(prevName, curName);

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

	TileSetInfo* info = FindInfo(mTileNameList->GetSelectItem());
	info->Type = CUtil::StringToTileType(label);

	mCSV->SetData(mTileNameList->GetSelectItem(), "TileType", std::string(label));
}

void CTileSetEditWindow::OnClickSaveCSV()
{
	if (mMapTileSet.empty())
	{
		return;
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

		TCHAR fileName[MAX_PATH] = {};
		_wsplitpath_s(filePath, 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);
		char convertFileName[MAX_PATH] = {};
		length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertFileName, length, 0, 0);

		if (!mCSV->SaveCSVFullPath(convertFullPath))
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
		TCHAR fileName[MAX_PATH] = {};
		_wsplitpath_s(filePath, 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);
		char convertFileName[MAX_PATH] = {};
		int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertFileName, length, 0, 0);

		MessageBox(nullptr, TEXT("타일셋 로드 완료"), TEXT("Success"), MB_OK);
	}
}

TileSetInfo* CTileSetEditWindow::FindInfo(const std::string& name)
{
	auto find = mMapTileSet.find(name);

	if (find == mMapTileSet.end())
	{
		return nullptr;
	}

	return find->second;
}

bool CTileSetEditWindow::DeleteInfo(const std::string& name)
{
	auto iter = mMapTileSet.begin();
	auto iterEnd = mMapTileSet.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->first == name)
		{
			SAFE_DELETE(iter->second);
			mMapTileSet.erase(iter);
			return true;
		}
	}
	return false;
}
