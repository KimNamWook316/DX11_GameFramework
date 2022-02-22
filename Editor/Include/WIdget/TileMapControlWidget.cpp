#include "TileMapControlWidget.h"
#include "../Window/TileSetEditWindow.h"
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
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"

CTileMapControlWidget::CTileMapControlWidget()	:
	mTileImageWidget(nullptr),
	mTileShapeWidget(nullptr),
	mTileCountWidget(nullptr),
	mTileSizeWidget(nullptr),
	mCreateMapButton(nullptr),
	mTileTypeWidget(nullptr),
	mDefaultFrameButon(nullptr),
	mOpenTileSetEditButton(nullptr)
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

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Tile Image");
	mTileImageWidget = AddWidget<CIMGUIImage>("TileImage");

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Create");
	mTileShapeWidget = AddWidget<CIMGUIComboBox>("Tile Shape");
	mTileCountWidget = AddWidget<CIMGUIInputInt2>("Tile Count");
	mTileSizeWidget = AddWidget<CIMGUIInputInt2>("Tile Size");
	mCreateMapButton = AddWidget<CIMGUIButton>("Create Map", 0.f, 0.f);

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Tile Info");
	mTileTypeWidget = AddWidget<CIMGUIComboBox>("Tile Type");
	mDefaultFrameButon = AddWidget<CIMGUIButton>("Set", 0.f, 0.f);
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");
	text = AddWidget<CIMGUIText>("text");
	text->SetText("Default Frame");

	AddWidget<CIMGUISeperator>("sperator");
	

	// CallBack
	mOpenTileSetEditButton->SetClickCallBack(this, &CTileMapControlWidget::OnClickOpenTileSetEdit);
	mCreateMapButton->SetClickCallBack(this, &CTileMapControlWidget::OnClickCreateMap);
	mDefaultFrameButon->SetClickCallBack(this, &CTileMapControlWidget::OnClickDefaultFrame);

	return true;
}

void CTileMapControlWidget::OnClickLoadImage()
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

		// 현재 TileMap에 Tile Material이 있는지
		CTileMapComponent* tileComp = static_cast<CTileMapComponent*>(mComponent);

		CMaterial* tileMat = tileComp->GetTileMaterial();
		std::string matName = mComponent->GetName() + "TileMapMaterial";

		// 없다면 머티리얼 생성
		if (!tileMat)
		{
			if (!mComponent->GetScene()->GetResource()->CreateMaterial<CMaterial>(matName))
			{
				MessageBox(nullptr, TEXT("타일 머티리얼 생성 실패."), TEXT("Failed"), MB_OK);
				return;
			}
			tileMat = mComponent->GetScene()->GetResource()->FindMaterial(matName);
			tileMat->SetShader("TileMapShader");
			tileMat->SetRenderState("AlphaBlend");
			tileComp->SetTileMaterial(tileMat);
		}
		tileComp->AddTileTextureFullPath(0, (int)eBufferShaderTypeFlags::Pixel, mComponent->GetName(), filePath);

		CTexture* tex = tileComp->GetTileMaterial()->GetTexture();
		mTileImageWidget->SetTexture(tex);
	}
}

void CTileMapControlWidget::OnClickCreateMap()
{
	if (!static_cast<CTileMapComponent*>(mComponent)->GetTileMaterial())
	{
		MessageBox(nullptr, TEXT("현재 타일 이미지가 없습니다."), TEXT("Creation Failed"), MB_OK);
		return;
	}

	if (-1 == mTileShapeWidget->GetSelectIndex())
	{
		MessageBox(nullptr, TEXT("타일 모양을 선택해야 합니다."), TEXT("Creation Failed"), MB_OK);
		return;
	}

	eTileShape shape = CUtil::StringToTileShape(mTileShapeWidget->GetSelectItem());

	int countX = mTileCountWidget->GetX();
	int countY = mTileCountWidget->GetY();
	int sizeX = mTileSizeWidget->GetX();
	int sizeY = mTileSizeWidget->GetY();

	if (0 >= countX || 0 >= countY || 0 >= sizeX || 0 >= sizeY)
	{
		MessageBox(nullptr, TEXT("타일 개수와 크기가 올바르지 않습니다."), TEXT("Creation Failed"), MB_OK);
		return;
	}

	static_cast<CTileMapComponent*>(mComponent)->CreateTile(shape, countX, countY, Vector2(sizeX, sizeY));
}

void CTileMapControlWidget::OnSelectType(int idx, const char* label)
{
}

void CTileMapControlWidget::OnChangeFrameStart(int val[2])
{
	CTileMapComponent* comp =  static_cast<CTileMapComponent*>(mComponent);

	if (comp->GetTileMaterial()->IsTextureEmpty())
	{
		return;
	}
	mTileImageWidget->SetImageStart((float)val[0], (float)val[1]);
}

void CTileMapControlWidget::OnChangeFrameEnd(int val[2])
{
	CTileMapComponent* comp =  static_cast<CTileMapComponent*>(mComponent);

	if (comp->GetTileMaterial()->IsTextureEmpty())
	{
		return;
	}
	mTileImageWidget->SetImageEnd((float)val[0], (float)val[1]);
}

void CTileMapControlWidget::OnClickDefaultFrame()
{
	CTileMapComponent* comp =  static_cast<CTileMapComponent*>(mComponent);

	if (comp->GetTileMaterial()->IsTextureEmpty())
	{
		return;
	}

	Vector2 start = mFrameStartWidget->GetValue();
	Vector2 end = mFrameEndWidget->GetValue();

	mTileImageWidget->SetImageStart(start.x, start.y);
	mTileImageWidget->SetImageEnd(end.x, end.y);
}

void CTileMapControlWidget::OnClickOpenTileSetEdit()
{
	CIMGUIManager::GetInst()->AddWindow<CTileSetEditWindow>("TileSet Editor");
}
