#include "SpriteWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUIListBox.h"
#include "IMGUIImage.h"
#include "IMGUICheckBox.h"
#include "IMGUIRadioButton.h"
#include "IMGUITree.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "PathManager.h"
#include "Input.h"
#include "Device.h"

CSpriteWindow::CSpriteWindow()	:
	mImage(nullptr),
	mCropImage(nullptr),
	mAnimationFrameList(nullptr),
	mAnimationList(nullptr),
	mAnimationNameInput(nullptr),
	mCropImageTextWidth(nullptr),
	mCropImageTextHeight(nullptr)
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
	
	CIMGUIText* text = AddWidget<CIMGUIText>("Text");
	text->SetText("Original Image");
	
	mImage = AddWidget<CIMGUIImage>("SpriteOrigin", 200.f, 200.f);

	text = AddWidget<CIMGUIText>("Text");
	text->SetText("Crop Image");
	mCropImage = AddWidget<CIMGUIImage>("SpriteCrop", 200.f, 200.f);

	text = AddWidget<CIMGUIText>("CropXText");
	text->SetText("Width : ");

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");
	mCropImageTextWidth = AddWidget<CIMGUIText>("CropX");
	mCropImageTextWidth->SetText("0.f");

	line = AddWidget<CIMGUISameLine>("Line");
	text = AddWidget<CIMGUIText>("CropYText");
	text->SetText("Height : ");

	line = AddWidget<CIMGUISameLine>("Line");
	mCropImageTextHeight = AddWidget<CIMGUIText>("CropY");
	mCropImageTextHeight->SetText("0.f");

	text = AddWidget<CIMGUIText>("Start");
	text->SetText("Start : ");

	AddWidget<CIMGUISameLine>("Line");
	mCropImageTextStart = AddWidget<CIMGUIText>("Start");
	mCropImageTextStart->SetText("0.f");

	AddWidget<CIMGUISameLine>("Line");
	text = AddWidget<CIMGUIText>("End");
	text->SetText("End : ");

	AddWidget<CIMGUISameLine>("Line");
	mCropImageTextEnd = AddWidget<CIMGUIText>("End");
	mCropImageTextEnd->SetText("0.f");

	CIMGUILabel* label = AddWidget<CIMGUILabel>("Animation List Name", 200.f, 30.f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0);

	line = AddWidget<CIMGUISameLine>("Line");
	mAnimationNameInput = AddWidget<CIMGUITextInput>("Animation Name Input", 80.f, 30.f);
	mAnimationNameInput->SetHideName(true);
	mAnimationNameInput->SetHintText("ex)Anim1");

	line = AddWidget<CIMGUISameLine>("Line");
	line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(330.f);

	label = AddWidget<CIMGUILabel>("Animation Frame Name", 200.f, 30.f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0);

	mAnimationList = AddWidget<CIMGUIListBox>("AnimationList", 200.f, 300.f);
	mAnimationList->SetHideName(true);
	mAnimationList->SetPageItemCount(6);
	mAnimationList->SetSelectCallBack<CSpriteWindow>(this, &CSpriteWindow::OnSelectAnimationList);
	
	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("Add Anim", 80.f, 30.f);
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickAddAnimation);

	line = AddWidget<CIMGUISameLine>("Line");

	mAnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 200.f, 300.f);
	mAnimationFrameList->SetHideName(true);
	mAnimationFrameList->SetPageItemCount(6);
	mAnimationFrameList->SetSelectCallBack<CSpriteWindow>(this, &CSpriteWindow::OnSelectAnimationFrame);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("Add Frame", 80.f, 30.f);
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
	openFile.lpstrFilter = TEXT("��� ����\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
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

		// Image Widget�� ��������Ʈ ���
		mImage->SetTextureFullPath(convertFileName, filePath);
		mCropImage->SetTextureFullPath(convertFileName, filePath);
		
		// �� ��������Ʈ ������Ʈ�� ���
		mSpriteEditObject->GetSpriteComponent()->SetTextureFullPath(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			convertFileName, filePath);
		// Texture ũ�� ���� ������ �°� ����
		mSpriteEditObject->GetSpriteComponent()->SetWorldScale((float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
			(float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.f);
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
	// ����ó��
	if (mAnimationNameInput->IsEmpty() || mSpriteEditObject == nullptr)
	{
		return;
	}

	const char* text = mAnimationNameInput->GetTextMultiByte();

	if (mAnimationList->IsItemExist(text))
	{
		// TODO : popup
		return;
	}
	
	// ���� �� ���ҽ� ������ �ִϸ��̼� �߰�
	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();

	if (!resource->CreateAnimationSequence2D(text, mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()))
	{
		return;
	}

	mAnimationList->AddItem(text);
}

void CSpriteWindow::OnClickAddAnimationFrame()
{
	if (!mSpriteEditObject)
	{
		return;
	}

	int selectAnimIdx = mAnimationList->GetSelectIndex();

	// � �ִϸ��̼ǵ� ���õ��� �ʾ��� ��
	if (selectAnimIdx == -1)
	{
		return;
	}

	// �� ���ҽ����� �ִϸ��̼� ������ ����
	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
	CAnimationSequence2D* anim = resource->FindAnimationSequence2D(mAnimationList->GetItem(selectAnimIdx));

	// ������ ���� : mCropImage�� ��ǥ�� ���ĵ� ��ǥ�� ������ �ֱ� ������ ������
	Vector2 frameImageSize = mCropImage->GetImageEnd() - mCropImage->GetImageStart();
	frameImageSize.x = abs(frameImageSize.x);
	frameImageSize.y = abs(frameImageSize.y);

	// ������ �߰�
	anim->AddFrame(mCropImage->GetImageStart(), frameImageSize);
	
	// UI�� �߰�
	int frameCount = anim->GetFrameCount();
	
	char frameName[32] = {};
	sprintf_s(frameName, "%d", frameCount);

	mAnimationFrameList->AddItem(frameName);
}

void CSpriteWindow::OnSelectAnimationList(int idx, const char* item)
{
	// ���� ������ ����Ʈ �ʱ�ȭ
	mAnimationFrameList->Clear();

	CAnimationSequence2D* anim = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence2D(item);
	
	// ���� ���õ� �ִϸ��̼��� �����Ӹ���Ʈ ���
	int size = (int)anim->GetFrameCount();
	for (int i = 0; i < size; ++i)
	{
		mAnimationFrameList->AddItem(std::to_string(i));
	}

	// ���� ���õ� �ִϸ��̼��� �ؽ��Ŀ� ���� �ִϸ��̼��� �ؽ��İ� �ٸ��� ��ü
	CTexture* selectAnimTexture = anim->GetTexture();

	if (selectAnimTexture != mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture())
	{
		mSpriteEditObject->GetSpriteComponent()->SetTexture(0, 0, 
			(int)eConstantBufferShaderTypeFlags::Pixel, selectAnimTexture->GetName(), selectAnimTexture);

		mSpriteEditObject->GetSpriteComponent()->SetWorldScale((float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
			(float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.f);
	}
	
	if (selectAnimTexture != mImage->GetTexture())
	{
		mImage->SetTexture(anim->GetTexture());
	}

	if (selectAnimTexture != mCropImage->GetTexture())
	{
		mCropImage->SetTexture(anim->GetTexture());
	}
}

void CSpriteWindow::OnSelectAnimationFrame(int idx, const char* item)
{
	// ����ó��
	if (mAnimationList->GetSelectIndex() == -1)
	{
		return;
	}

	CAnimationSequence2D* anim = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence2D(mAnimationList->GetSelectItem());
	
	AnimationFrameData data = anim->GetFrameData(idx);

	// �ִϸ��̼� ��ġ���� DirectX��ǥ��� ��ȯ �� Crop Imageâ ������Ʈ
	mCropStartPos = data.Start;
	mCropStartPos.y = mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight() - mCropStartPos.y;
	mCropEndPos.x = mCropStartPos.x + data.Size.x;
	mCropEndPos.y = mCropStartPos.y - data.Size.y;

	UpdateCropImage();
}

void CSpriteWindow::MoveCropPos(const float x, const float y)
{
	Vector2 val(x, y);
	mCropStartPos += val;
	mCropEndPos += val;

	UpdateCropImage();
}

void CSpriteWindow::UpdateCropImage()
{
	Vector2 topLeft;
	Vector2 botRight;

	// ���콺 ���ȴ� ��ġ�� ���� �»�, ���� ��ǥ ����
	if (mCropStartPos.x < mCropEndPos.x)
	{
		topLeft.x = mCropStartPos.x;
		botRight.x = mCropEndPos.x;
	}
	else
	{
		topLeft.x = mCropEndPos.x;
		botRight.x = mCropStartPos.x;
	}

	if (mCropStartPos.y > mCropEndPos.y)
	{
		topLeft.y = mCropStartPos.y;
		botRight.y = mCropEndPos.y;
	}
	else
	{
		topLeft.y = mCropEndPos.y;
		botRight.y = mCropStartPos.y;
	}
	
	// ���� ������ �Ǿ� �ִ� ������Ʈ�� �»�, ���� ��ǥ �޾ƿ���
	Vector3 objectPos = mSpriteEditObject->GetRelativePos();
	Vector3 objectScale = mSpriteEditObject->GetWorldScale();
	Vector3 objectPivot = mSpriteEditObject->GetPivot();

	Vector2 objTopLeft = Vector2(objectPos.x - (objectPivot.x * objectScale.x),
		(objectPos.y + objectScale.y) + (objectPivot.y * objectScale.y));

	Vector2 objBotRight = Vector2((objectPos.x + objectScale.x) + (objectPivot.x * objectScale.x),
		objectPos.y - (objectPivot.y * objectScale.y));

	// �ؽ��� ������ ���콺�� ���� ���
	if ((botRight.x < objTopLeft.x) ||
		(topLeft.x > objBotRight.x) ||
		(botRight.y > objTopLeft.y) ||
		(topLeft.y < objBotRight.y))
	{
		return;
	}
	else
	{
		// ���콺 �巡�� ���� �� �ؽ��Ŀ� ��ġ�� ������ ����
		if (topLeft.x < objTopLeft.x)
		{
			topLeft.x = objTopLeft.x;
		}
		if (botRight.x > objBotRight.x)
		{
			botRight.x = objBotRight.x;
		}
		if (topLeft.y > objTopLeft.y)
		{
			topLeft.y = objTopLeft.y;
		}
		if (botRight.y < objBotRight.y)
		{
			botRight.y = objBotRight.y;
		}
		
		// UV��ǥ ����
		mCropImage->SetImageStart(topLeft.x, objectScale.y - topLeft.y);
		mCropImage->SetImageEnd(botRight.x, objectScale.y - botRight.y);

		// �ʺ�, ���� �ؽ�Ʈ ����
		char buf[64] = {};
		float width = botRight.x - topLeft.x;
		float height = topLeft.y - botRight.y;

		sprintf_s(buf, "%.1f", width);
		mCropImageTextWidth->SetText(buf);
		sprintf_s(buf, "%.1f", height);
		mCropImageTextHeight->SetText(buf);
	}
}
