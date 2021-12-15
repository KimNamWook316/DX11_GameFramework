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
#include "IMGUIChild.h"
#include "IMGUIDummy.h"
#include "IMGUISeperator.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat.h"
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
	mWidthInput(nullptr),
	mHeightInput(nullptr),
	mStartXInput(nullptr),
	mStartYInput(nullptr),
	mAnimLoopCheckBox(nullptr),
	mAnimReverseCheckBox(nullptr),
	mPlayTimeInput(nullptr),
	mPlayScaleInput(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
}

bool CSpriteWindow::Init()
{
	// Adjust Window Size;
	ImGui::SetNextWindowSize(ImVec2(400.f, 500.f));

	CIMGUIWindow::Init();

	CIMGUIText* text = AddWidget<CIMGUIText>("animation");
	text->SetText("Animation");
	AddWidget<CIMGUISeperator>("sperator");

	// Col1. Animation List Column
	CIMGUIChild* animCol = AddWidget<CIMGUIChild>("animCol", 150.f, 550.f);
	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("line");

	text = animCol->AddWidget<CIMGUIText>("animList");
	text->SetText("Animation List");
	animCol->AddWidget<CIMGUISeperator>("sperator");

	// Col1. Animation ListBox
	mAnimationList = animCol->AddWidget<CIMGUIListBox>("AnimationList", 200.f, 300.f);
	mAnimationList->SetHideName(true);
	mAnimationList->SetPageItemCount(22);
	mAnimationList->SetSelectCallBack<CSpriteWindow>(this, &CSpriteWindow::OnSelectAnimationList);

	// Col1. Animation Name Input
	mAnimationNameInput = animCol->AddWidget<CIMGUITextInput>("Anim Name", 150.f, 100.f);
	mAnimationNameInput->SetHintText("Input Animation Name");

	// Col1. Animation Add, Delete Button 
	CIMGUIButton* button = animCol->AddWidget<CIMGUIButton>("Add", 0.f, 0.f);
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickAddAnimation);
	animCol->AddWidget<CIMGUISameLine>("line");
	button = animCol->AddWidget<CIMGUIButton>("Del", 0.f, 0.f);
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickDeleteAnimation);

	// Col2. Frame List Column
	CIMGUIChild* frameCol = AddWidget<CIMGUIChild>("frameCol", 150.f, 550.f);
	AddWidget<CIMGUISameLine>("line");

	text = frameCol->AddWidget<CIMGUIText>("frameList");
	text->SetText("Frame List");
	frameCol->AddWidget<CIMGUISeperator>("sperator");

	// Col2. Frame ListBox
	mAnimationFrameList = frameCol->AddWidget<CIMGUIListBox>("AnimationFrameList", 200.f, 300.f);
	mAnimationFrameList->SetHideName(true);
	mAnimationFrameList->SetPageItemCount(22);
	mAnimationFrameList->SetSelectCallBack<CSpriteWindow>(this, &CSpriteWindow::OnSelectAnimationFrame);

	// Col2. Frame Add/Del Button
	button = frameCol->AddWidget<CIMGUIButton>("Add", 0.f, 0.f);
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickAddAnimationFrame);
	frameCol->AddWidget<CIMGUISameLine>("line");
	button = frameCol->AddWidget<CIMGUIButton>("Del", 0.f, 0.f);
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickDeleteAnimationFrame);

	// Col3. Info & Edit Animation Coloumn
	CIMGUIChild* infoCol = AddWidget<CIMGUIChild>("infoCol", 400.f, 550.f);

	text = infoCol->AddWidget<CIMGUIText>("frameList");
	text->SetText("Edit");
	infoCol->AddWidget<CIMGUISameLine>("line");

	// Col3. Load Texture & Edit Sprite Button
	button = infoCol->AddWidget<CIMGUIButton>("Load");
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickLoadTexture);
	button->SetSize(0.f, 0.f);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("EditMode");
	button->SetClickCallBack<CSpriteWindow>(this, &CSpriteWindow::OnClickSpriteEdit);
	button->SetSize(0.f, 0.f);
	infoCol->AddWidget<CIMGUISeperator>("sperator");
	
	// Col3. CropImage
	text = infoCol->AddWidget<CIMGUIText>("Text");
	text->SetText("Crop Image");
	mCropImage = infoCol->AddWidget<CIMGUIImage>("SpriteCrop", 200.f, 200.f);
	infoCol->AddWidget<CIMGUISeperator>("sperator");

	// Col3. Image Poistion Edit Input
	text = infoCol->AddWidget<CIMGUIText>("text");
	text->SetText("Position");
	mWidthInput = infoCol->AddWidget<CIMGUIInputInt>("Width", 30.f, 0.f);
	mWidthInput->SetSize(200.f, 0.f);
	mHeightInput = infoCol->AddWidget<CIMGUIInputInt>("Height", 30.f, 0.f);
	mHeightInput->SetSize(200.f, 0.f);
	mStartXInput = infoCol->AddWidget<CIMGUIInputInt>("StartX", 30.f, 0.f);
	mStartXInput->SetSize(200.f, 0.f);
	mStartYInput = infoCol->AddWidget<CIMGUIInputInt>("StartY", 30.f, 0.f);
	mStartYInput->SetSize(200.f, 0.f);
	infoCol->AddWidget<CIMGUISeperator>("sperator");
	
	// Col3. Animation Setting, Play & Stop Button
	text = infoCol->AddWidget<CIMGUIText>("text");
	text->SetText("Animation Value");
	mAnimLoopCheckBox = infoCol->AddWidget<CIMGUICheckBox>("Loop", 30.f, 0.f);
	mAnimLoopCheckBox->AddCheckInfo("Loop");
	infoCol->AddWidget<CIMGUISameLine>("line");
	mAnimReverseCheckBox = infoCol->AddWidget<CIMGUICheckBox>("Reverse", 30.f, 0.f);
	mAnimReverseCheckBox->AddCheckInfo("Reverse");
	mPlayTimeInput = infoCol->AddWidget<CIMGUIInputFloat>("Play Time", 200.f, 0.f);
	mPlayScaleInput = infoCol->AddWidget<CIMGUIInputFloat>("Play Scale", 200.f, 0.f);
	button = infoCol->AddWidget<CIMGUIButton>("Play", 0.f, 0.f);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Stop", 0.f, 0.f);
	infoCol->AddWidget<CIMGUISeperator>("sperator");

	// Col3. Save & Load
	text = infoCol->AddWidget<CIMGUIText>("text");
	text->SetText("Save & Load");
	button = infoCol->AddWidget<CIMGUIButton>("Save", 0.f, 0.f);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Load", 0.f, 0.f);

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

void CSpriteWindow::OnClickDeleteAnimation()
{
}

void CSpriteWindow::OnClickDeleteAnimationFrame()
{
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
	
	// Drag Object �ش� ��ġ��

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

 //		// �ʺ�, ���� �ؽ�Ʈ ����
 //		char buf[64] = {};
 //		float width = botRight.x - topLeft.x;
 //		float height = topLeft.y - botRight.y;
 //
 //		sprintf_s(buf, "%.1f", width);
 //		mCropImageTextWidth->SetText(buf);
 //		sprintf_s(buf, "%.1f", height);
 //		mCropImageTextHeight->SetText(buf);
	}
}
