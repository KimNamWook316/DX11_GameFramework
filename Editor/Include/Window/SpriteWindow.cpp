#include "SpriteWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h";
#include "IMGUIListBox.h";
#include "IMGUIImage.h"
#include "IMGUICheckBox.h"
#include "IMGUIRadioButton.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "PathManager.h"

CSpriteWindow::CSpriteWindow()	:
	mImage(nullptr),
	mAnimationFrameList(nullptr),
	mAnimationList(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
}

bool CSpriteWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUIButton* button = AddWidget<CIMGUIButton>("Load Texture");
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickLoadTexture);

	AddWidget<CIMGUISameLine>("line");

	button = AddWidget<CIMGUIButton>("Edit Sprite");
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickSpriteEdit);

	mImage = AddWidget<CIMGUIImage>("SpriteOrigin", 200.f, 200.f);
	
	CIMGUILabel* label = AddWidget<CIMGUILabel>("Animation List Name", 200.f, 30.f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(300.f);

	label = AddWidget<CIMGUILabel>("Animation Frame Name", 200.f, 30.f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0);

	mAnimationList = AddWidget<CIMGUIListBox>("AnimationList", 200.f, 300.f);
	mAnimationList->SetHideName(true);
	mAnimationList->SetPageItemCount(6);
	
	line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(300.f);

	mAnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 200.f, 300.f);
	mAnimationFrameList->SetHideName(true);
	mAnimationFrameList->SetPageItemCount(6);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("Add Animation Frame", 80.f, 30.f);
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickAddAnimationFrame);

	return true;
}

void CSpriteWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CSpriteWindow::OnClickLoadTexture()
{
	if (CEditorManager::GetInst()->GetEditMode() != eEditMode::Sprite)
	{
		return;
	}

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

		// Image Widget에 스프라이트 등록
		mImage->SetTextureFullPath(convertFileName, filePath);
		
		// 씬 스프라이트 오브젝트에 등록
		mSpriteEditObject->GetSpriteComponent()->SetTextureFullPath(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			convertFileName, filePath);
	}
}

void CSpriteWindow::OnClickSpriteEdit()
{
	CEditorManager::GetInst()->SetEditMode(eEditMode::Sprite);

	if (!mSpriteEditObject)
	{
		mSpriteEditObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
	}
}

void CSpriteWindow::OnClickAddAnimation()
{
}

void CSpriteWindow::OnClickAddAnimationFrame()
{
}
