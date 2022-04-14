#include "CameraControlWidget.h"
#include "Component/CameraComponent.h"
#include "Scene/CameraManager.h"
#include "IMGUIText.h"
#include "IMGUISeperator.h"
#include "IMGUIButton.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIInputFloat2.h"
#include "IMGUISliderInt.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "Scene/Scene.h"

CCameraControlWidget::CCameraControlWidget()	:
	mScale(nullptr),
	mPos(nullptr),
	mResetButton(nullptr),
	mCameraType(nullptr),
	mIsPlayer(nullptr),
	mViewPortRatioX(nullptr),
	mViewPortRatioY(nullptr),
	mViewPortCenter(nullptr),
	mSetCurrentCameraButton(nullptr)
{
}

CCameraControlWidget::~CCameraControlWidget()
{
	static_cast<CSceneComponent*>(mComponent.Get())->DeleteCallBack("CameraWidget");
}

bool CCameraControlWidget::Init()
{
	// Widget
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("Camera Component");
	mScale = AddWidget<CIMGUIInputFloat3>("Camera Scale");
	mPos = AddWidget<CIMGUIInputFloat3>("Pos");
	mCameraType = AddWidget<CIMGUIComboBox>("Type");
	mIsPlayer = AddWidget<CIMGUICheckBox>("IsPlayer");
	mIsPlayer->AddCheckInfo("IsPlayer");
	mViewPortRatioX = AddWidget<CIMGUISliderInt>("ViewPort Ratio X");
	mViewPortRatioY = AddWidget<CIMGUISliderInt>("ViewPort Ratio Y");
	mViewPortCenter = AddWidget<CIMGUIButton>("ViewPort Center", 0.f, 0.f);
	mResetButton = AddWidget<CIMGUIButton>("Reset", 0.f, 0.f);
	mSetCurrentCameraButton = AddWidget<CIMGUIButton>("SetCurrent", 0.f, 0.f);
	AddWidget<CIMGUISeperator>("sep");

	// Initial Value
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	Vector3 scale = cam->GetScale();
	mScale->SetVal(scale.x, scale.y, scale.z);
	Vector3 pos = cam->GetWorldPos();
	mPos->SetVal(pos.x, pos.y, pos.z);

	for (int i = 0; i < (int)eCameraType::Max; ++i)
	{
		mCameraType->AddItem(CUtil::CameraTypeToString((eCameraType)i));
	}

	// 이 오브젝트가 플레이어 오브젝트일 경우
	CGameObject* obj = mComponent->GetGameObject();
	if (obj == obj->GetScene()->GetPlayerObj())
	{
		mIsPlayer->SetCheck(0, true);
	}

	mCameraType->SetCurrentItem(CUtil::CameraTypeToString(cam->GetCameraType()));

	mViewPortRatioX->SetMin(0);
	mViewPortRatioY->SetMin(0);
	mViewPortRatioX->SetMax(10);
	mViewPortRatioY->SetMax(10);
	if (eCameraType::Camera2D == cam->GetCameraType())
	{
		mViewPortRatioX->SetValue(cam->GetRatio().x);
		mViewPortRatioY->SetValue(cam->GetRatio().y);
	}

	// CallBack
	mPos->SetCallBack(this, &CCameraControlWidget::OnChangePos);
	mScale->SetCallBack(this, &CCameraControlWidget::OnChangeScale);
	mResetButton->SetClickCallBack(this, &CCameraControlWidget::OnClickReset);
	mCameraType->SetSelectCallBack(this, &CCameraControlWidget::OnSelectCameraType);
	mIsPlayer->SetCallBackIdx(this, &CCameraControlWidget::OnCheckIsPlayer);
	mViewPortRatioX->SetCallBack(this, &CCameraControlWidget::OnChangeViewPortRatioX);
	mViewPortRatioY->SetCallBack(this, &CCameraControlWidget::OnChangeViewPortRatioY);
	mViewPortCenter->SetClickCallBack(this, &CCameraControlWidget::OnClickViewPortCenter);
	mSetCurrentCameraButton->SetClickCallBack(this, &CCameraControlWidget::OnClickSetCurrentCamera);

	static_cast<CSceneComponent*>(mComponent.Get())->AddCallBack("CameraWidget", this, &CCameraControlWidget::OnChangeState);

	return true;
}

void CCameraControlWidget::Render()
{
	CIMGUIWidgetList::Render();
}

void CCameraControlWidget::OnChangeScale(float val[3])
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	cam->SetScale(mScale->GetValue());
}

void CCameraControlWidget::OnChangePos(float val[3])
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	cam->SetWorldPos(mPos->GetValue());
}

void CCameraControlWidget::OnClickReset()
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	mScale->SetVal(1.f, 1.f, 1.f);
	mPos->SetVal(0.f, 0.f, 0.f);
	cam->SetScale(1.f, 1.f, 1.f);
	cam->SetWorldPos(0.f, 0.f, 0.f);
}

void CCameraControlWidget::OnSelectCameraType(int idx, const char* label)
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	cam->SetCameraType(CUtil::StringToCameraType(label));
}

void CCameraControlWidget::OnCheckIsPlayer(int idx, bool bCheck)
{
	CGameObject* obj = mComponent->GetGameObject();

	if (bCheck)
	{
		obj->GetScene()->SetPlayerObj(obj);
		obj->GetScene()->GetCameraManager()->KeepCamera();
		obj->GetScene()->GetCameraManager()->SetCurrentCamera((CCameraComponent*)mComponent.Get());
	}
	else
	{
		obj->GetScene()->SetPlayerObj(nullptr);
		obj->GetScene()->GetCameraManager()->ReturnCamera();
	}
}

void CCameraControlWidget::OnChangeViewPortRatioX(int val)
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	if (eCameraType::Camera2D == cam->GetCameraType())
	{
		cam->SetViewPortByRatio(val, cam->GetRatio().y);
	}
}

void CCameraControlWidget::OnChangeViewPortRatioY(int val)
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	if (eCameraType::Camera2D == cam->GetCameraType())
	{
		cam->SetViewPortByRatio(cam->GetRatio().x, val);
	}
}

void CCameraControlWidget::OnClickViewPortCenter()
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());
	if (eCameraType::Camera2D == cam->GetCameraType())
	{
		cam->OnViewportCenter();
		mViewPortRatioX->SetValue(cam->GetRatio().x);
		mViewPortRatioY->SetValue(cam->GetRatio().y);
	}
}

void CCameraControlWidget::OnClickSetCurrentCamera()
{
	CCameraComponent* cam = static_cast<CCameraComponent*>(mComponent.Get());

	mComponent->GetScene()->GetCameraManager()->KeepCamera();
	mComponent->GetScene()->GetCameraManager()->SetCurrentCamera(cam);
}

void CCameraControlWidget::OnChangeState()
{
	CSceneComponent* com = static_cast<CSceneComponent*>(mComponent.Get());
	Vector3 scale = com->GetWorldScale();
	Vector3 pos = com->GetWorldPos();

	mScale->SetVal(scale.x, scale.y, scale.z);
	mPos->SetVal(pos.x, pos.y, pos.z);
}
