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
	mAnimationInstance(nullptr),
	mSpriteWidthText(nullptr),
	mSpriteHeightText(nullptr),
	mAutoSplitNameInput(nullptr),
	mSplitCountXInput(nullptr),
	mSplitCountYInput(nullptr)
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
	
	// Sprite Info
	text = infoCol->AddWidget<CIMGUIText>("Text");
	text->SetText("Sprite Info");
	text = infoCol->AddWidget<CIMGUIText>("Text");
	text->SetText("Width : ");
	infoCol->AddWidget<CIMGUISameLine>("line");
	mSpriteWidthText = infoCol->AddWidget<CIMGUIText>("widthText");
	mSpriteWidthText->SetText("0");
	infoCol->AddWidget<CIMGUISameLine>("line");
	text = infoCol->AddWidget<CIMGUIText>("Text");
	text->SetText("Height : ");
	infoCol->AddWidget<CIMGUISameLine>("line");
	mSpriteHeightText = infoCol->AddWidget<CIMGUIText>("heightText");
	mSpriteHeightText->SetText("0");
	infoCol->AddWidget<CIMGUISeperator>("sperator");

	// Auto Split
	text = infoCol->AddWidget<CIMGUIText>("Text");
	text->SetText("Auto Split");
	mSplitCountXInput = infoCol->AddWidget<CIMGUIInputInt>("Frame Count", 30.f, 0.f);
	mSplitCountXInput->SetSize(0.f, 0.f);
	mSplitCountYInput = infoCol->AddWidget<CIMGUIInputInt>("Anim Count", 30.f, 0.f);
	mSplitCountYInput->SetSize(0.f, 0.f);
	mAutoSplitNameInput = infoCol->AddWidget<CIMGUITextInput>("Animation Name", 100.f, 100.f);
	mAutoSplitNameInput->SetHintText("Animation Name");
	button = infoCol->AddWidget<CIMGUIButton>("Split Auto");
	button->SetClickCallBack(this, &CSpriteWindow::OnClickAutoSplit);
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
	mAnimLoopCheckBox->SetCallBack(this, &CSpriteWindow::OnClickLoopCheckBox);
	infoCol->AddWidget<CIMGUISameLine>("line");
	mAnimReverseCheckBox = infoCol->AddWidget<CIMGUICheckBox>("Reverse", 30.f, 0.f);
	mAnimReverseCheckBox->AddCheckInfo("Reverse");
	mAnimReverseCheckBox->SetCallBack(this, &CSpriteWindow::OnClickReverseCheckBox);
	mPlayTimeInput = infoCol->AddWidget<CIMGUIInputFloat>("Play Time", 200.f, 0.f);
	mPlayTimeInput->SetCallBack(this, &CSpriteWindow::OnPlayTimeInputChanged);
	mPlayScaleInput = infoCol->AddWidget<CIMGUIInputFloat>("Play Scale", 200.f, 0.f);
	mPlayScaleInput->SetCallBack(this, &CSpriteWindow::OnPlayScaleInputChanged);
	button = infoCol->AddWidget<CIMGUIButton>("Play", 0.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickPlayAnimation);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Stop", 0.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickStopAnimation);
	infoCol->AddWidget<CIMGUISeperator>("sperator");

	// Col3. Save & Load
	text = infoCol->AddWidget<CIMGUIText>("text");
	text->SetText("Save & Load");
	button = infoCol->AddWidget<CIMGUIButton>("Save Sequence", 130.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickSaveSequence);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Load Sequence", 130.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickLoadSequence);
	button = infoCol->AddWidget<CIMGUIButton>("Save All", 130.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickSaveAllSequence);
	button = infoCol->AddWidget<CIMGUIButton>("Save Animation", 130.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickSaveAnimation);
	infoCol->AddWidget<CIMGUISameLine>("line");
	button = infoCol->AddWidget<CIMGUIButton>("Load Animation", 130.f, 0.f);
	button->SetClickCallBack(this, &CSpriteWindow::OnClickLoadAnimation);

	// Animation Instance
	mAnimationInstance = new CAnimationSequence2DInstance;
	mAnimationInstance->Init();
	mAnimationInstance->Start();
	mAnimationInstance->Stop();

	// ShortCut Key
	CInput::GetInst()->CreateKey("Enter", VK_RETURN);
	CInput::GetInst()->SetKeyCallBack("Enter", eKeyState::KeyState_Down, this, &CSpriteWindow::OnDownReturnKey);

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

			// UI 업데이트
			mCropImage->SetImageStart(frameData.Start.x, frameData.Start.y);
			mCropImage->SetImageEnd(frameData.Start.x + frameData.Size.x, frameData.Start.y + frameData.Size.y);

			// Drag Object 현재 프레임으로 이동
			CEditorManager::GetInst()->GetDragObject()->SetWorldPos(frameData.Start.x, 
				mSpriteEditObject->GetWorldScale().y - frameData.Start.y - frameData.Size.y, 0.f);
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
		mCropImage->SetTextureFullPath(convertFileName, filePath);

		// 씬 스프라이트 오브젝트에 등록
		mSpriteEditObject->GetSpriteComponent()->SetTextureFullPath(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			convertFileName, filePath);

		// Texture 크기 실제 비율에 맞게 수정
		CTexture* texture = mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture();
		mSpriteEditObject->GetSpriteComponent()->SetWorldScale((float)texture->GetWidth(),
			(float)texture->GetHeight(), 1.f);

		// Sprite Info 갱신
		char buf[32] = {};
		sprintf_s(buf, "%d", texture->GetWidth());
		mSpriteWidthText->SetText(buf);
		sprintf_s(buf, "%d", texture->GetHeight());
		mSpriteHeightText->SetText(buf);
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
	// 예외처리
	if (mAnimationNameInput->IsEmpty() || mSpriteEditObject == nullptr)
	{
		return;
	}

	const char* text = mAnimationNameInput->GetTextMultiByte();

	if (mAnimationList->IsItemExist(text))
	{
		return;
	}
	
	// 현재 씬 리소스 가져와 애니메이션 추가
	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();

	if (!resource->CreateAnimationSequence2D(text, mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()))
	{
		return;
	}

	// UI update
	mAnimationList->AddItem(text);

	// Animation Instance에 저장
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

	// 어떤 애니메이션도 선택되지 않았을 때
	if (selectAnimIdx == -1)
	{
		return;
	}

	// 씬 리소스에서 애니메이션 시퀀스 얻어옴
	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
	CAnimationSequence2D* anim = resource->FindAnimationSequence2D(mAnimationList->GetItem(selectAnimIdx));

	// 사이즈 설정 : mCropImage가 좌표순 정렬된 좌표를 가지고 있기 때문에 가져옴
	Vector2 frameImageSize = mCropImage->GetImageEnd() - mCropImage->GetImageStart();
	frameImageSize.x = abs(frameImageSize.x);
	frameImageSize.y = abs(frameImageSize.y);

	// 프레임 추가
	anim->AddFrame(mCropImage->GetImageStart(), frameImageSize);
	
	// UI에 추가
	int frameCount = anim->GetFrameCount();
	
	char frameName[32] = {};
	sprintf_s(frameName, "%d", frameCount);

	mAnimationFrameList->AddItem(frameName);

	// UI 업데이트
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

	// 재생중인 애니메이션 프레임 정보 업데이트
	if (mAnimationInstance->IsPlay())
	{
		mAnimationInstance->ReplayCurrentAnimation();
	}

	// UI clear
	clearFrameUI();
}

void CSpriteWindow::OnSelectAnimationList(int idx, const char* item)
{
	// 현재 애니메이션 변경
	mAnimationInstance->SetCurrentAnimation(item);
	int frameCount = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount();

	// 이전 프레임 리스트 초기화
	mAnimationFrameList->Clear();
	
	// 현재 선택된 애니메이션의 프레임리스트 출력
	for (int i = 0; i < frameCount; ++i)
	{
		mAnimationFrameList->AddItem(std::to_string(i));
	}

	// 현재 선택된 애니메이션의 텍스쳐와 이전 애니메이션의 텍스쳐가 다르면 교체
	CTexture* selectAnimTexture = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture();

	if (selectAnimTexture != mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture())
	{
		mSpriteEditObject->GetSpriteComponent()->SetTexture(0, 0, 
			(int)eConstantBufferShaderTypeFlags::Pixel, selectAnimTexture->GetName(), selectAnimTexture);

		mSpriteEditObject->GetSpriteComponent()->SetWorldScale((float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
			(float)mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.f);

		// Sprite Info 갱신
		char buf[32] = {};
		sprintf_s(buf, "%d", selectAnimTexture->GetWidth());
		mSpriteWidthText->SetText(buf);
		sprintf_s(buf, "%d", selectAnimTexture->GetHeight());
		mSpriteHeightText->SetText(buf);
	}
	
	if (selectAnimTexture != mCropImage->GetTexture())
	{
		mCropImage->SetTexture(selectAnimTexture);
	}

	// 현재 Animation의 0번 프레임 UI에 출력
	if (mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount() != 0)
	{
		AnimationFrameData data = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);
		mCropImage->SetImageStart(data.Start.x, data.Start.y);
		mCropImage->SetImageEnd(data.Start.x + data.Size.x, data.Start.y + data.Size.y);

		CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
		float height = (float)selectAnimTexture->GetHeight();

		// DragObject 0번 프레임 위치로
		dragObj->SetWorldScale(data.Size.x, data.Size.y, 1.f);
		dragObj->SetWorldPos(data.Start.x, height - data.Start.y - data.Size.y, 0.f);
	}

	updateFrameUI();
	updateAnimationUI();
}

void CSpriteWindow::OnSelectAnimationFrame(int idx, const char* item)
{
	// 예외처리
	if (mAnimationList->GetSelectIndex() == -1)
	{
		return;
	}

	CAnimationSequence2D* anim = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence2D(mAnimationList->GetSelectItem());
	
	AnimationFrameData data = anim->GetFrameData(idx);

	// 애니메이션 위치정보 DirectX좌표계로 변환 후 Crop Image창 업데이트
	mCropStartPos = data.Start;
	mCropStartPos.y = mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight() - mCropStartPos.y;
	mCropEndPos.x = mCropStartPos.x + data.Size.x;
	mCropEndPos.y = mCropStartPos.y - data.Size.y;

	// Image UI 업데이트
	UpdateCropImage();
	
	// Drag Object 해당 위치로
	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	dragObj->SetWorldScale(abs(mCropEndPos.x - mCropStartPos.x), abs(mCropEndPos.y - mCropStartPos.y), 1.f);
	dragObj->SetWorldPos(mCropStartPos.x, mCropStartPos.y - dragObj->GetWorldScale().y, 0.f);

	// Poisiton UI 업데이트
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
	// Crop UI Update
	mCropImage->SetImageEnd(mCropImage->GetImageStart().x + val, mCropImage->GetImageEnd().y);
	
	// Drag Objct
	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	Vector3 scale = dragObj->GetRelativeScale();
	Vector3 pos = dragObj->GetWorldPos();
	if (scale.x < 0)
	{
		dragObj->SetWorldScale(-scale.x, scale.y, scale.z);
		pos.x += scale.x;
 		dragObj->SetWorldPos(pos);
	}		
	dragObj->SetWorldScale((float)val, scale.y, scale.z);
	
	// Animation Sequence FrameData
	int curFrame = mAnimationFrameList->GetSelectIndex();
	if (curFrame == -1)
	{
		return;
	}
	mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->SetWidth(curFrame, (float)val);
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
	dragObj->SetWorldScale(scale.x, -(float)val, scale.z);

	// Animation Sequence FrameData
	int curFrame = mAnimationFrameList->GetSelectIndex();
	if (curFrame == -1)
	{
		return;
	}
	mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->SetHeight(curFrame, (float)val);
}

void CSpriteWindow::OnStartXInputChanged(int val)
{
	int width = (int)(mCropImage->GetImageEnd().x - mCropImage->GetImageStart().x);
	mCropImage->SetImageStart((float)val, mCropImage->GetImageStart().y);
	mCropImage->SetImageEnd((float)val + width, mCropImage->GetImageEnd().y);

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	Vector3 scale = dragObj->GetRelativeScale();
	Vector3 pos = dragObj->GetWorldPos();
	if (scale.x < 0)
	{
		dragObj->SetWorldScale(-scale.x, scale.y, scale.z);
		pos.x += scale.x;
 		dragObj->SetWorldPos(pos);
	}
	pos.x = (float)val;
	dragObj->SetWorldPos(pos);

	// Animation Sequence FrameData
	int curFrame = mAnimationFrameList->GetSelectIndex();
	if (curFrame == -1)
	{
		return;
	}
	mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->SetStartX(curFrame, (float)val);
}

void CSpriteWindow::OnStartYInputChanged(int val)
{
	int height = int(mCropImage->GetImageEnd().y - mCropImage->GetImageStart().y);
	mCropImage->SetImageStart(mCropImage->GetImageStart().x, (float)val);
	mCropImage->SetImageEnd(mCropImage->GetImageEnd().x, height + (float)val);

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
	pos.y = (float)(mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight() - val);
	dragObj->SetWorldPos(pos);

	// Animation Sequence FrameData
	int curFrame = mAnimationFrameList->GetSelectIndex();
	if (curFrame == -1)
	{
		return;
	}
	mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->SetStartY(curFrame, (float)val);
}

void CSpriteWindow::OnClickLoopCheckBox(const char* name, bool bLoop)
{
	mAnimationInstance->GetCurrentAnimation()->EnableLoop(bLoop);
}

void CSpriteWindow::OnClickReverseCheckBox(const char* name, bool bLoop)
{
	mAnimationInstance->GetCurrentAnimation()->EnableReverse(bLoop);
}

void CSpriteWindow::OnPlayTimeInputChanged(float val)
{
	mAnimationInstance->GetCurrentAnimation()->SetPlayTime(val);
}

void CSpriteWindow::OnPlayScaleInputChanged(float val)
{
	mAnimationInstance->GetCurrentAnimation()->SetPlayScale(val);
}

void CSpriteWindow::OnClickSaveSequence()
{
	int selectAnimIdx = mAnimationList->GetSelectIndex();
	if (selectAnimIdx == -1)
	{
		return;
	}

	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("모든 파일\0*.*\0Sequence File\0*.sqc");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetSaveFileName(&openFile) != 0)
	{
		FILE* pFile = nullptr;
		
		char fullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);
		
		mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->SaveFullPath(fullPath);
	}
}

// Animation Folder에 자동으로 저장
void CSpriteWindow::OnClickSaveAllSequence()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ANIMATION_PATH);

	std::vector<std::string> sequenceNames;
	mAnimationInstance->GetAnimationNames(sequenceNames);

	bool bSuccess = true;
	
	for (size_t i = 0; i < sequenceNames.size(); ++i)
	{
		char fullPath[MAX_PATH] = {};
		CAnimationSequence2D* sequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence2D(sequenceNames[i]);
		
		strcpy_s(fullPath, info->PathMultibyte);
		strcat_s(fullPath, sequenceNames[i].c_str());
		strcat_s(fullPath, ".sqc");
		
		if (!sequence->SaveFullPath(fullPath))
		{
			bSuccess = false;
			break;
		}
	}

	if (bSuccess)
	{
		MessageBox(nullptr, TEXT("Saved In Deafult Animation Folder"), TEXT("Saved"), MB_OK);
	}
	else
	{
		MessageBox(nullptr, TEXT("Failed To Save Sequence"), TEXT("Failed"), MB_OK);
	}
}

void CSpriteWindow::OnClickLoadSequence()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("모든 파일\0*.*\0Sequence File\0*.sqc");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;
	openFile.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (GetOpenFileName(&openFile) != 0)
	{
		CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
		std::string sequenceName;

		FILE* pFile = nullptr;
		char fullPathMutliByte[MAX_PATH] = {};

		// 선택된 파일이 여러개가 아닌 경우
		if (openFile.nFileExtension != 0)
		{
			int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPathMutliByte, length, 0, 0);

			resource->LoadSequence2DFullPath(sequenceName, fullPathMutliByte);
			
			mAnimationInstance->AddAnimation(sequenceName, sequenceName);
		}
		// 여러 개 파일이 선택된 경우
		// TODO : 너무 파일 이름이 길어질 경우 로드되지 않음. OpenFile 이슈
		else
		{
			int fileCount = 0;
			TCHAR slash[2] = L"\\";
			TCHAR filePath1[MAX_PATH] = {};

			lstrcpy(filePath1, filePath);
			lstrcat(filePath1, slash);
			
			TCHAR* fileName = openFile.lpstrFile + openFile.nFileOffset;
			int fileNameLength = lstrlen(fileName);

			// 더 이상 파일 이름 받아올 수 없을 때까지 반복
			while (*fileName != '\0')
			{
				TCHAR fullPath[MAX_PATH] = {};
				lstrcpy(fullPath, filePath1);
				lstrcat(fullPath, fileName);
				fileName += (fileNameLength + 1);
				fileNameLength = lstrlen(fileName);

				int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, 0, 0, 0, 0);
				WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMutliByte, length, 0, 0);

				resource->LoadSequence2DFullPath(sequenceName, fullPathMutliByte);

				mAnimationInstance->AddAnimation(sequenceName, sequenceName);
				mAnimationList->AddItem(resource->FindAnimationSequence2D(sequenceName)->GetName());
			}
		}

		if (!mSpriteEditObject)
		{
			mSpriteEditObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
			mSpriteEditObject->Init();
		}
	}
}

void CSpriteWindow::OnClickSaveAnimation()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("모든 파일\0*.*\0Anim Instance File\0*.anim");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetSaveFileName(&openFile) != 0)
	{
		FILE* pFile = nullptr;

		char fullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);
		
		mAnimationInstance->Save(fullPath);
	}
}

void CSpriteWindow::OnClickLoadAnimation()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("모든 파일\0*.*\0Anim Instance File\0*.anim");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetOpenFileName(&openFile) != 0)
	{
		FILE* pFile = nullptr;

		char fullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);

		mAnimationInstance->Load(fullPath);
		mAnimationInstance->Init();
		
		if (!mAnimationList->IsEmpty())
		{
			mAnimationList->Clear();
		}

		std::vector<std::string> mVecNames;
		mAnimationInstance->GetAnimationNames(mVecNames);

		size_t size = mVecNames.size();
		for (size_t i = 0; i < size; ++i)
		{
			mAnimationList->AddItem(mVecNames[i]);
		}
		mAnimationList->Sort(true);

		if (!mAnimationFrameList->IsEmpty())
		{
			mAnimationFrameList->Clear();
		}
	
		CTexture* selectAnimTexture = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture();

		if (!mSpriteEditObject)
		{
			mSpriteEditObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
			mSpriteEditObject->Init();
		}

		mSpriteEditObject->GetSpriteComponent()->SetTexture(0, 0, 
			(int)eConstantBufferShaderTypeFlags::Pixel, selectAnimTexture->GetName(), selectAnimTexture);

		// Texture 크기 실제 비율에 맞게 수정
		CTexture* texture = mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture();
		mSpriteEditObject->GetSpriteComponent()->SetWorldScale((float)texture->GetWidth(),
			(float)texture->GetHeight(), 1.f);

		// Sprite Info 갱신
		char buf[32] = {};
		sprintf_s(buf, "%d", texture->GetWidth());
		mSpriteWidthText->SetText(buf);
		sprintf_s(buf, "%d", texture->GetHeight());
		mSpriteHeightText->SetText(buf);

		// 현재 Animation의 0번 프레임 UI에 출력
		if (mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount() != 0)
		{
			AnimationFrameData data = mAnimationInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);
			mCropImage->SetImageStart(data.Start.x, data.Start.y);
			mCropImage->SetImageEnd(data.Start.x + data.Size.x, data.Start.y + data.Size.y);

			CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
			float height = (float)selectAnimTexture->GetHeight();

			// DragObject 0번 프레임 위치로
			dragObj->SetWorldScale(data.Size.x, data.Size.y, 1.f);
			dragObj->SetWorldPos(data.Start.x, data.Start.y - data.Size.y, 0.f);
		}
	}
}

void CSpriteWindow::OnDownReturnKey(float dummy)
{
	if (eEditMode::Sprite == CEditorManager::GetInst()->GetEditMode())
	{
		OnClickAddAnimationFrame();
	}
}

void CSpriteWindow::OnClickAutoSplit()
{
	if (mSpriteEditObject)
	{
		CTexture* texture = mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture();
		const char* animName = mAutoSplitNameInput->GetTextMultiByte();

		int frameCount = mSplitCountXInput->GetVal();
		int animCount = mSplitCountYInput->GetVal();

		// 예외처리
		if (!texture)
			return;
		if (!animName)
			return;
		if (animCount < 1)
			return;
		if (frameCount < 1)
			return;

		unsigned int width = texture->GetWidth();
		unsigned int height = texture->GetHeight();

		int frameWidth = (int)(std::round((float)width / (float)frameCount));
		int frameHeight = (int)(std::round((float)height / (float)animCount));

		CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();

		for (int y = 0; y < animCount; ++y)
		{
			// 시퀀스 생성
			std::string sequenceName = animName;
			sequenceName += std::to_string(y);

			if (!resource->CreateAnimationSequence2D(sequenceName, mSpriteEditObject->GetSpriteComponent()->GetMaterial()->GetTexture()))
			{
				return;
			}

			// UI update
			mAnimationList->AddItem(sequenceName);

			// Animation Instance에 저장
			mAnimationInstance->AddAnimation(sequenceName, sequenceName, mAnimLoopCheckBox->GetCheck(0),
				mPlayTimeInput->GetVal(), mPlayScaleInput->GetVal(), 
				mAnimReverseCheckBox->GetCheck(0));

			CAnimationSequence2D* curSequence = resource->FindAnimationSequence2D(sequenceName);

			for (int x = 0; x < frameCount; ++x)
			{
				// 프레임 추가
				Vector2 startPos = Vector2(x * frameWidth, y * frameHeight);
				Vector2 size = Vector2(frameWidth, frameHeight);
				curSequence->AddFrame(startPos, size);
			}
		}
	}
}

void CSpriteWindow::MoveCropPos(const float x, const float y)
{
	Vector2 val(x, y);
	mCropStartPos += val;
	mCropEndPos += val;

	UpdateCropImage();
}

void CSpriteWindow::updateAnimList()
{
}

void CSpriteWindow::updateFrameList()
{
}

void CSpriteWindow::updateFrameUI()
{
	Vector2 imageStart = mCropImage->GetImageStart();
	Vector2 imageEnd = mCropImage->GetImageEnd();
	
	mStartXInput->SetVal((int)imageStart.x);
	mStartYInput->SetVal((int)imageStart.y);

	mWidthInput->SetVal((int)(imageEnd.x - imageStart.x));
	mHeightInput->SetVal((int)(imageEnd.y - imageStart.y));
}

void CSpriteWindow::updateAnimationUI()
{
	CAnimationSequence2DData* curAnim = mAnimationInstance->GetCurrentAnimation();
	mAnimLoopCheckBox->SetCheck(0, curAnim->IsLoop());
	mAnimReverseCheckBox->SetCheck(0, curAnim->IsReverse());
	mPlayTimeInput->SetVal(curAnim->GetPlayTime());
	mPlayScaleInput->SetVal(curAnim->GetPlayScale());
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

	// 마우스 눌렸던 위치와 비교해 좌상, 우하 좌표 설정
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
	
	// 실제 렌더링 되어 있는 오브젝트의 좌상, 우하 좌표 받아오기
	Vector3 objectPos = mSpriteEditObject->GetRelativePos();
	Vector3 objectScale = mSpriteEditObject->GetWorldScale();
	Vector3 objectPivot = mSpriteEditObject->GetPivot();

	Vector2 objTopLeft = Vector2(objectPos.x - (objectPivot.x * objectScale.x),
		(objectPos.y + objectScale.y) + (objectPivot.y * objectScale.y));

	Vector2 objBotRight = Vector2((objectPos.x + objectScale.x) + (objectPivot.x * objectScale.x),
		objectPos.y - (objectPivot.y * objectScale.y));

	// 텍스쳐 범위에 마우스가 없을 경우
	if ((botRight.x < objTopLeft.x) ||
		(topLeft.x > objBotRight.x) ||
		(botRight.y > objTopLeft.y) ||
		(topLeft.y < objBotRight.y))
	{
		return;
	}
	else
	{
		// 마우스 드래그 범위 중 텍스쳐와 겹치는 범위만 추출
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
		
		// UV좌표 설정
		mCropImage->SetImageStart(topLeft.x, objectScale.y - topLeft.y);
		mCropImage->SetImageEnd(botRight.x, objectScale.y - botRight.y);
	}

	updateFrameUI();
}
