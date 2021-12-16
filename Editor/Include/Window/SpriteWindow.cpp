#include "Animation/AnimationSequence2DInstance.h"
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
#include "../Object/DragObject.h"

CSpriteWindow::CSpriteWindow()	:
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
	mPlayScaleInput(nullptr),
	mAnimationInstance(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
	SAFE_DELETE(mAnimationInstance);
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
	mAnimationNameInput = animCol->AddWidget<CIMGUITextInput>("Anim Name", 70.f, 100.f);
	mAnimationNameInput->SetHideName(true);
	mAnimationNameInput->SetHintText("Animation Name");
	animCol->AddWidget<CIMGUISameLine>("line");

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
	mWidthInput->SetCallBack(this, &CSpriteWindow::OnWidthInputChanged);
	mHeightInput = infoCol->AddWidget<CIMGUIInputInt>("Height", 30.f, 0.f);
	mHeightInput->SetSize(200.f, 0.f);
	mHeightInput->SetCallBack(this, &CSpriteWindow::OnHeightInputChanged);
	mStartXInput = infoCol->AddWidget<CIMGUIInputInt>("StartX", 30.f, 0.f);
	mStartXInput->SetSize(200.f, 0.f);
	mStartXInput->SetCallBack(this, &CSpriteWindow::OnStartXInputChanged);
	mStartYInput = infoCol->AddWidget<CIMGUIInputInt>("StartY", 30.f, 0.f);
	mStartYInput->SetSize(200.f, 0.f);
	mStartYInput->SetCallBack(this, &CSpriteWindow::OnStartYInputChanged);
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
	button->SetClickCallBack(this, &CSpriteWindow::OnClickPlayAnimation);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Stop", 0.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickStopAnimation);
	infoCol->AddWidget<CIMGUISeperator>("sperator");

	// Col3. Save & Load
	text = infoCol->AddWidget<CIMGUIText>("text");
	text->SetText("Save & Load");
	button = infoCol->AddWidget<CIMGUIButton>("Save", 0.f, 0.f);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Load", 0.f, 0.f);

	// Animation Instance
	mAnimationInstance = new CAnimationSequence2DInstance;
	mAnimationInstance->Init();
	mAnimationInstance->Start();
	mAnimationInstance->Stop();

	return true;
}

void CSpriteWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);

	mAnimationInstance->Update(deltaTime);

	if (mAnimationInstance->IsPlay())
	{
		CAnimationSequence2DData* animData = mAnimationInstance->GetCurrentAnimation();

		if (animData)
		{
			int frame = animData->GetCurrentFrame();

			AnimationFrameData frameData = animData->GetAnimationSequence()->GetFrameData(frame);

			// UI ������Ʈ
			mCropImage->SetImageStart(frameData.Start.x, frameData.Start.y);
			mCropImage->SetImageEnd(frameData.Start.x + frameData.Size.x, frameData.Start.y + frameData.Size.y);

			// Drag Object ���� ���������� �̵�
			CEditorManager::GetInst()->GetDragObject()->SetWorldPos(frameData.Start.x, 
				mSpriteEditObject->GetWorldScale().y - frameData.Start.y, 0.f);
		}
	}
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
		return;
	}
	
	// ���� �� ���ҽ� ������ �ִϸ��̼� �߰�
	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();

	if (!resource->CreateAnimationSequence2D(text, mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()))
	{
		return;
	}

	// UI update
	mAnimationList->AddItem(text);

	// Animation Instance�� ����
	mAnimationInstance->AddAnimation(text, text, mAnimLoopCheckBox->GetCheck(0),
		mPlayTimeInput->GetVal(), mPlayScaleInput->GetVal(), 
		mAnimReverseCheckBox->GetCheck(0));
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

	// UI ������Ʈ
	updateFrameUI();
}

void CSpriteWindow::OnClickDeleteAnimation()
{
	int selectIdx = mAnimationList->GetSelectIndex();

	if (-1 == selectIdx)
	{
		return;
	}

	mAnimationInstance->DeleteAnimation(mAnimationList->GetSelectItem());

	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
	resource->ReleaseAnimationSequence2D(mAnimationList->GetSelectItem());

	mAnimationList->DeleteItem(selectIdx);
	mAnimationFrameList->Clear();
}

void CSpriteWindow::OnClickDeleteAnimationFrame()
{
	int selectAnimIdx = mAnimationList->GetSelectIndex();
	int selectIdx = mAnimationFrameList->GetSelectIndex();

	if (-1 == selectIdx || -1 == selectAnimIdx)
	{
		return;
	}
	
	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
	CAnimationSequence2D* anim = resource->FindAnimationSequence2D(mAnimationList->GetItem(selectAnimIdx));

	anim->DeleteFrame(selectIdx);

	mAnimationFrameList->Clear();

	char frameName[32] = {};

	for (int i = 0; i < anim->GetFrameCount(); ++i)
	{
		sprintf_s(frameName, "%d", i + 1);
		mAnimationFrameList->AddItem(frameName);
	}

	// ������� �ִϸ��̼� ������ ���� ������Ʈ
	if (mAnimationInstance->IsPlay())
	{
		mAnimationInstance->ReplayCurrentAnimation();
	}

	// UI clear
	clearFrameUI();
}

void CSpriteWindow::OnSelectAnimationList(int idx, const char* item)
{
	// ���� �ִϸ��̼� ����
	mAnimationInstance->SetCurrentAnimation(item);
	int frameCount = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount();

	// ���� ������ ����Ʈ �ʱ�ȭ
	mAnimationFrameList->Clear();
	
	// ���� ���õ� �ִϸ��̼��� �����Ӹ���Ʈ ���
	for (int i = 0; i < frameCount; ++i)
	{
		mAnimationFrameList->AddItem(std::to_string(i));
	}

	// ���� ���õ� �ִϸ��̼��� �ؽ��Ŀ� ���� �ִϸ��̼��� �ؽ��İ� �ٸ��� ��ü
	CTexture* selectAnimTexture = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture();

	if (selectAnimTexture != mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture())
	{
		mSpriteEditObject->GetSpriteComponent()->SetTexture(0, 0, 
			(int)eConstantBufferShaderTypeFlags::Pixel, selectAnimTexture->GetName(), selectAnimTexture);

		mSpriteEditObject->GetSpriteComponent()->SetWorldScale((float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
			(float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.f);
	}
	
	if (selectAnimTexture != mCropImage->GetTexture())
	{
		mCropImage->SetTexture(selectAnimTexture);
	}

	// ���� Animation�� 0�� ������ UI�� ���
	if (mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount() != 0)
	{
		AnimationFrameData data = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);
		mCropImage->SetImageStart(data.Start.x, data.Start.y);
		mCropImage->SetImageEnd(data.Start.x + data.Size.x, data.Start.y + data.Size.y);

		// DragObject 0�� ������ ��ġ��
		CEditorManager::GetInst()->GetDragObject()->SetWorldPos(data.Start.x, data.Start.y, 0.f);
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
	CEditorManager::GetInst()->GetDragObject()->SetWorldPos(mCropStartPos.x, mCropStartPos.y , 0.f);

	// Image UI ������Ʈ
	UpdateCropImage();

	// Poisiton UI ������Ʈ
	updateFrameUI();
}

void CSpriteWindow::OnClickPlayAnimation()
{
	if (mAnimationInstance->GetCurrentAnimation() != nullptr)
	{
		mAnimationInstance->ReplayCurrentAnimation();
		mAnimationInstance->Play();
	}
}

void CSpriteWindow::OnClickStopAnimation()
{
	if (mAnimationInstance->GetCurrentAnimation() != nullptr)
	{
		mAnimationInstance->Stop();
	}
}

void CSpriteWindow::OnWidthInputChanged(int val)
{
	mCropImage->SetImageEnd(mCropImage->GetImageStart().x + val, mCropImage->GetImageEnd().y);
	
	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	Vector3 scale = dragObj->GetRelativeScale();
	Vector3 pos = dragObj->GetWorldPos();
	if (scale.x < 0)
	{
		dragObj->SetWorldScale(-scale.x, scale.y, scale.z);
		pos.x += scale.x;
 		dragObj->SetWorldPos(pos);
	}		
	dragObj->SetWorldScale(val, scale.y, scale.z);
}

void CSpriteWindow::OnHeightInputChanged(int val)
{
	mCropImage->SetImageEnd(mCropImage->GetImageEnd().x, mCropImage->GetImageStart().y + val);

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	Vector3 scale = dragObj->GetRelativeScale();
	Vector3 pos = dragObj->GetWorldPos();
	if (scale.y > 0)
	{
		scale.y *= -1;
		dragObj->SetWorldScale(scale.x, scale.y, scale.z);
		pos.y -= scale.y;
		dragObj->SetWorldPos(pos);
	}
	dragObj->SetWorldScale(scale.x, -val, scale.z);
}

void CSpriteWindow::OnStartXInputChanged(int val)
{
	int width = mCropImage->GetImageEnd().x - mCropImage->GetImageStart().x;
	mCropImage->SetImageStart(val, mCropImage->GetImageStart().y);
	mCropImage->SetImageEnd(val + width, mCropImage->GetImageEnd().y);

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	Vector3 scale = dragObj->GetRelativeScale();
	Vector3 pos = dragObj->GetWorldPos();
	if (scale.x < 0)
	{
		dragObj->SetWorldScale(-scale.x, scale.y, scale.z);
		pos.x += scale.x;
 		dragObj->SetWorldPos(pos);
	}
	pos.x = val;
	dragObj->SetWorldPos(pos);
}

void CSpriteWindow::OnStartYInputChanged(int val)
{
	int height = mCropImage->GetImageEnd().y - mCropImage->GetImageStart().y;
	mCropImage->SetImageStart(mCropImage->GetImageStart().x, val);
	mCropImage->SetImageEnd(mCropImage->GetImageEnd().x, height + val);

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	Vector3 scale = dragObj->GetRelativeScale();
	Vector3 pos = dragObj->GetWorldPos();
	if (scale.y > 0)
	{
		scale.y *= -1;
		dragObj->SetWorldScale(scale.x, scale.y, scale.z);
		pos.y -= scale.y;
		dragObj->SetWorldPos(pos);
	}
	pos.y = mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight() - val;
	dragObj->SetWorldPos(pos);
}

void CSpriteWindow::OnClickLoopCheckBox(bool bLoop)
{
}

void CSpriteWindow::OnClickReverseCheckBox(bool bLoop)
{
}

void CSpriteWindow::OnClickSave()
{
}

void CSpriteWindow::MoveCropPos(const float x, const float y)
{
	Vector2 val(x, y);
	mCropStartPos += val;
	mCropEndPos += val;

	UpdateCropImage();
}

void CSpriteWindow::updateFrameUI()
{
	Vector2 imageStart = mCropImage->GetImageStart();
	Vector2 imageEnd = mCropImage->GetImageEnd();
	
	mStartXInput->SetVal(imageStart.x);
	mStartYInput->SetVal(imageStart.y);

	mWidthInput->SetVal(imageEnd.x - imageStart.x);
	mHeightInput->SetVal(imageEnd.y - imageStart.y);
}

void CSpriteWindow::updateAnimationUI()
{
}

void CSpriteWindow::clearFrameUI()
{
	mStartXInput->SetVal(0);
	mStartYInput->SetVal(0);
	mWidthInput->SetVal(0);
	mHeightInput->SetVal(0);
}

void CSpriteWindow::clearAnimationUI()
{
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
	}

	updateFrameUI();
}