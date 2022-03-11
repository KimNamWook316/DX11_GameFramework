#include "SpriteControlWidget.h"
#include "IMGUIText.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIColor4.h"
#include "IMGUISliderFloat.h"
#include "IMGUICheckBox.h"
#include "IMGUITextInput.h"
#include "IMGUITree.h"
#include "IMGUIButton.h"
#include "IMGUISliderInt.h"
#include "IMGUIComboBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "Component/SpriteComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "Resource/Animation/AnimationSequence2D.h"

CSpriteControlWidget::CSpriteControlWidget()	:
	mPositionWidget(nullptr),
	mScaleWidget(nullptr),
	mRotWidget(nullptr),
	mPivotWidget(nullptr),
	mColorWidget(nullptr),
	mTransparencyWidget(nullptr),
	mOpacityWidget(nullptr),
	mInheritScaleWidget(nullptr),
	mInheritPosWidget(nullptr),
	mInheritRotWidget(nullptr),
	mLoadAnimationWidget(nullptr),
	mAnimationWidget(nullptr),
	mAnimationNameWidget(nullptr),
	mAnimationFrameWidget(nullptr),
	mAnimationPlayButton(nullptr),
	mAnimationListWidget(nullptr)
{
}

CSpriteControlWidget::~CSpriteControlWidget()
{
	if (!mComponent)
	{
		return;
	}

	CAnimationSequence2DInstance* animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();

	if (!animInst)
	{
		return;
	}

	std::vector<std::string> animNames;
	animInst->GetAnimationNames(animNames);

	size_t size = animNames.size();

	// 콜백 삭제
	for (size_t i = 0; i < size; ++i)
	{
		CAnimationSequence2DData* data = animInst->FindAnimation(animNames[i]);

		int frameCount = data->GetAnimationSequence()->GetFrameCount();

		for (int j = 0; j < frameCount; ++j)
		{
			data->DeleteNotify(animNames[i]);
		}
	}

}

bool CSpriteControlWidget::Init()
{
	// Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("Sprite Component");
	text->SetText("Sprite Component");
	mPositionWidget = AddWidget<CIMGUIInputFloat3>("Position");
	mScaleWidget = AddWidget<CIMGUIInputFloat3>("Scale");
	mRotWidget = AddWidget<CIMGUIInputFloat3>("Rotation");
	mPivotWidget = AddWidget<CIMGUIInputFloat3>("Pivot");
	mColorWidget = AddWidget<CIMGUIColor4>("BaseColor");
	mTransparencyWidget = AddWidget<CIMGUICheckBox>("Enable Transparency");
	mOpacityWidget = AddWidget<CIMGUISliderFloat>("Opacity");
	text = AddWidget<CIMGUIText>("text");
	text->SetText("Transform Inheritance");
	mInheritScaleWidget = AddWidget<CIMGUICheckBox>("InheritScale");
	mInheritPosWidget = AddWidget<CIMGUICheckBox>("InheritScale");
	mInheritRotWidget = AddWidget<CIMGUICheckBox>("InheritScale");
	mAnimationWidget = AddWidget<CIMGUITree>("Animation");
	mLoadAnimationWidget = mAnimationWidget->AddWidget<CIMGUIButton>("Load", 50.f, 0.f);
	mLoadAnimationWidget->SetClickCallBack(this, &CSpriteControlWidget::OnClickLoadAnimation);
	mAnimationNameWidget = mAnimationWidget->AddWidget<CIMGUITextInput>("Animation Name");
	mAnimationNameWidget->ReadOnly(true);
	mAnimationListWidget = mAnimationWidget->AddWidget<CIMGUIComboBox>("Animation List", 300.f, 30.f);
	mAnimationPlayButton = mAnimationWidget->AddWidget<CIMGUIButton>("Play", 0.f, 0.f);
	mAnimationWidget->AddWidget<CIMGUISameLine>("Line");
	mAnimationFrameWidget = mAnimationWidget->AddWidget<CIMGUISliderInt>("Frame", 200.f);
	AddWidget<CIMGUISeperator>("sperator");

	// CallBack
	mPositionWidget->SetCallBack(this, &CSpriteControlWidget::OnChangePosition);	
	mScaleWidget->SetCallBack(this, &CSpriteControlWidget::OnChangeScale);
	mRotWidget->SetCallBack(this, &CSpriteControlWidget::OnChangeRot);
	mPivotWidget->SetCallBack(this, &CSpriteControlWidget::OnChangePivot);
	mColorWidget->SetCallBack(this, &CSpriteControlWidget::OnChangeBaseColor);
	mTransparencyWidget->SetCallBackLabel(this, &CSpriteControlWidget::OnChangeTransparency);
	mOpacityWidget->SetCallBack(this, &CSpriteControlWidget::OnChangeOpacity);
	mInheritScaleWidget->SetCallBackLabel(this, &CSpriteControlWidget::OnChangeInheritScale);
	mInheritPosWidget->SetCallBackLabel(this, &CSpriteControlWidget::OnChangeInheritPos);
	mInheritRotWidget->SetCallBackLabel(this, &CSpriteControlWidget::OnChangeInheritRot);
	mAnimationFrameWidget->SetCallBack(this, &CSpriteControlWidget::OnChangeAnimationFrame);
	mAnimationPlayButton->SetClickCallBack(this, &CSpriteControlWidget::OnClickPlayAnimation);
	mAnimationListWidget->SetSelectCallBack(this, &CSpriteControlWidget::OnSelectAnimationList);

	// Initial Value
	CSpriteComponent* comp = (CSpriteComponent*)mComponent;

	Vector3 pos = comp->GetRelativePos();
	Vector3 scale = comp->GetRelativeScale();
	Vector3 rot = comp->GetRelativeRot();
	Vector4 baseColor = comp->GetMaterial()->GetBaseColor();
	mPositionWidget->SetVal(pos.x, pos.y, pos.z);
	mScaleWidget->SetVal(scale.x, scale.y, scale.z);
	mRotWidget->SetVal(rot.x, rot.y, rot.z);
	mColorWidget->SetRGBA(baseColor.x, baseColor.y, baseColor.z, baseColor.w);
	mTransparencyWidget->AddCheckInfo("Tranparency");
	mTransparencyWidget->SetCheck(0, comp->GetIsTransparentActive());
	mOpacityWidget->SetValue(comp->GetMaterial()->GetOpacity());

	Vector3 pivot = comp->GetPivot();
	mPivotWidget->EnableInputRange(true);
	mPivotWidget->SetMin(0.f);
	mPivotWidget->SetMax(1.f);
	mPivotWidget->SetX(pivot.x);
	mPivotWidget->SetY(pivot.y);
	mPivotWidget->SetZ(pivot.z);

	mInheritPosWidget->SetColNum(3);
	mInheritRotWidget->SetColNum(3);
	mInheritScaleWidget->AddCheckInfo("Scale");
	mInheritRotWidget->AddCheckInfo("RotX");
	mInheritRotWidget->AddCheckInfo("RotY");
	mInheritRotWidget->AddCheckInfo("RotZ");
	mInheritPosWidget->AddCheckInfo("PosX");
	mInheritPosWidget->AddCheckInfo("PosY");
	mInheritPosWidget->AddCheckInfo("PosZ");

	mInheritScaleWidget->SetCheck(0, comp->GetInheritScale());
	mInheritRotWidget->SetCheck(0, comp->GetInheritRotX());
	mInheritRotWidget->SetCheck(1, comp->GetInheritRotY());
	mInheritRotWidget->SetCheck(2, comp->GetInheritRotZ());
	mInheritPosWidget->SetCheck(0, comp->GetInheritPosX());
	mInheritPosWidget->SetCheck(1, comp->GetInheritPosY());
	mInheritPosWidget->SetCheck(2, comp->GetInheritPosZ());

	return true;
}

void CSpriteControlWidget::OnChangePosition(float value[3])
{
	static_cast<CSceneComponent*>(mComponent)->SetRelativePos(value[0], value[1], value[2]);
}

void CSpriteControlWidget::OnChangeScale(float value[3])
{
	static_cast<CSceneComponent*>(mComponent)->SetRelativeScale(value[0], value[1], value[2]);
}

void CSpriteControlWidget::OnChangeRot(float value[3])
{
	static_cast<CSceneComponent*>(mComponent)->SetRelativeRot(value[0], value[1], value[2]);
}

void CSpriteControlWidget::OnChangePivot(float value[3])
{
	static_cast<CSceneComponent*>(mComponent)->SetPivot(value[0], value[1], value[2]);
}

void CSpriteControlWidget::OnChangeBaseColor(float val[4])
{
	static_cast<CSpriteComponent*>(mComponent)->GetMaterial()->SetBaseColor(val[0], val[1], val[2], val[3]);
}

void CSpriteControlWidget::OnChangeTransparency(const char* tag, bool bChecked)
{
	static_cast<CSpriteComponent*>(mComponent)->GetMaterial()->SetTransparency(bChecked);
}

void CSpriteControlWidget::OnChangeOpacity(float val)
{
	static_cast<CSpriteComponent*>(mComponent)->GetMaterial()->SetOpacity(val);
}

void CSpriteControlWidget::OnChangeInheritScale(const char* tag, bool bChecked)
{
	static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritScale(bChecked);
}

void CSpriteControlWidget::OnChangeInheritRot(const char* tag, bool bChecked)
{
	if (strcmp(tag, "RotX") == 0)
	{
		static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritRotX(bChecked);
	}
	else if (strcmp(tag, "RotY") == 0)
	{
		static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritRotY(bChecked);
	}
	else if (strcmp(tag, "RotX") == 0)
	{
		static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritRotZ(bChecked);
	}
}

void CSpriteControlWidget::OnChangeInheritPos(const char* tag, bool bChecked)
{
	if (strcmp(tag, "PosX") == 0)
	{
		static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritPosX(bChecked);
	}
	else if (strcmp(tag, "PosY") == 0)
	{
		static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritPosY(bChecked);
	}
	else if (strcmp(tag, "PosX") == 0)
	{
		static_cast<CSpriteComponent*>(mComponent)->SetBoolInheritPosZ(bChecked);
	}
}

void CSpriteControlWidget::OnClickLoadAnimation()
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

		static_cast<CSpriteComponent*>(mComponent)->LoadAnimationInstanceFullPath(fullPath);

		// Animation 초기화
		CAnimationSequence2DInstance* animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();
		animInst->Start();
		animInst->Play();

		// Size 조절
		Vector2 spriteSize = animInst->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0).Size;
		static_cast<CSpriteComponent*>(mComponent)->SetWorldScale(spriteSize.x, spriteSize.y, 1.f);
		mScaleWidget->SetX(spriteSize.x);
		mScaleWidget->SetY(spriteSize.y);

		// TODO : 위젯 업데이트
		// TODO : filePath만 Name Widget에 띄우게
		mAnimationNameWidget->SetText(fullPath);

		// Animation List
		mAnimationListWidget->Clear();

		std::vector<std::string> animNames;
		animInst->GetAnimationNames(animNames);

		size_t size = animNames.size();
		for (size_t i = 0; i < size; ++i)
		{
			mAnimationListWidget->AddItem(animNames[i]);
		}
		mAnimationListWidget->Sort(true);
		
		std::string curAnimName = animInst->GetCurrentAnimation()->GetName();
		mAnimationListWidget->SetCurrentItem(curAnimName);

		// Frame Widget
		mAnimationFrameWidget->SetMax(animInst->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount() - 1);

		// CallBack 등록
		for (size_t i = 0; i < size; ++i)
		{
			CAnimationSequence2DData* data = animInst->FindAnimation(animNames[i]);

			int frameCount = data->GetAnimationSequence()->GetFrameCount();
			
			for (int j = 0; j < frameCount; ++j)
			{
				animInst->AddNotify(animNames[i], j, this, &CSpriteControlWidget::OnUpdateAnimInstFrame);
			}
		}
	}
}

void CSpriteControlWidget::OnChangeAnimationFrame(int frame)
{
	// Stop
	CAnimationSequence2DInstance* animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();
	animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();
	animInst->Stop();

	// Set Frame
	animInst->SetCurrentFrame(frame);
}

void CSpriteControlWidget::OnClickPlayAnimation()
{
	CAnimationSequence2DInstance* animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();

	if (!animInst)
	{
		return;
	}

	if (!animInst->GetCurrentAnimation()->IsLoop() && animInst->IsEndFrame())
	{
		animInst->Replay();
		return;
	}
	
	bool bPlay = animInst->IsPlay();

	if (bPlay)
	{
		animInst->Stop();
	}
	else
	{
		animInst->Play();
	}
}

void CSpriteControlWidget::OnSelectAnimationList(int idx, const char* name)
{
	CAnimationSequence2DInstance* animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();
	// Change Animation
	std::string animName = name;
	animInst->ChangeAnimation(animName);

	// Size
	Vector2 spriteSize = animInst->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0).Size;
	static_cast<CSpriteComponent*>(mComponent)->SetWorldScale(spriteSize.x, spriteSize.y, 1.f);
	mScaleWidget->SetX(spriteSize.x);
	mScaleWidget->SetY(spriteSize.y);

	// Frame
	mAnimationFrameWidget->SetValue(0);
	mAnimationFrameWidget->SetMax(animInst->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount() - 1);
}

void CSpriteControlWidget::OnUpdateAnimInstFrame()
{
	CAnimationSequence2DInstance* animInst = static_cast<CSpriteComponent*>(mComponent)->GetAnimationInstance();

	if (!animInst)
	{
		return;
	}

	int currentFrame = animInst->GetCurrentAnimation()->GetCurrentFrame();
	mAnimationFrameWidget->SetValue(currentFrame);
}
