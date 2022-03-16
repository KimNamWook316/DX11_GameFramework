#include "ColliderControlWidget.h"
#include "IMGUIManager.h"
#include "../Window/CollisionProfileWindow.h"
#include "IMGUIWindow.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneCollision.h"
#include "Component/ColliderComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderPixel.h"
#include "Collision/CollisionManager.h"
#include "IMGUIText.h"
#include "IMGUIComboBox.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputFloat2.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"

CColliderControlWidget::CColliderControlWidget()	:
	mColliderTypeWidget(nullptr),
	mOffsetWidget(nullptr),
	mCollsionProfileWidget(nullptr),
	mEditCollisionProfileWidget(nullptr),
	mTypeText(nullptr),
	mEditBox2DWidth(nullptr),
	mEditBox2DHeight(nullptr),
	mEditCircleRadius(nullptr),
	mSeperator(nullptr)
{
}

CColliderControlWidget::~CColliderControlWidget()
{
}

bool CColliderControlWidget::Init()
{
	// Widget 등록
	CIMGUIText* text = AddWidget<CIMGUIText>("text");
	text->SetText("Collider Component");

	// 한 오브젝트에 여러 종류의 Collider가 들어갈수도 있기 떄문에, Widget 이름을 ID로 따로 관리함
	// IMGUI::PushID를 이용하는게 맞는 방법임
	char name[64] = {};
	sprintf_s(name, "%d", mWidgetID);
	mColliderTypeWidget = AddWidget<CIMGUIComboBox>(name, 150.f, 30.f);
	mColliderTypeWidget->SetHideName(true);
	AddWidget<CIMGUISameLine>("line");
	text = AddWidget<CIMGUIText>("text");
	text->SetText("Collider Type");

	// 충돌체 공용 위젯
	sprintf_s(name, "%d", mWidgetID + 1);
	mOffsetWidget = AddWidget<CIMGUIInputFloat3>(name, 200.f, 30.f);
	mOffsetWidget->SetHideName(true);
	AddWidget<CIMGUISameLine>("line");

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Offset");

	sprintf_s(name, "%d", mWidgetID + 1);
	mCollsionProfileWidget = AddWidget<CIMGUIComboBox>(name, 150.f, 30.f);
	mCollsionProfileWidget->SetHideName(true);

	AddWidget<CIMGUISameLine>("line");

	text = AddWidget<CIMGUIText>("text");
	text->SetText("Profile");

	AddWidget<CIMGUISameLine>("line");
	mEditCollisionProfileWidget = AddWidget<CIMGUIButton>("Edit Profile", 0.f, 0.f);

	// 충돌체 종류별 위젯
	switch (meType)
	{
	case eColliderType::Box2D:
	{
		mTypeText = AddWidget<CIMGUIText>("text");
		mTypeText->SetText("BoxInfo");
		mEditBox2DWidth = AddWidget<CIMGUIInputFloat>("Width");
		mEditBox2DHeight = AddWidget<CIMGUIInputFloat>("Height");
		mEditBox2DWidth->SetStep(1.f);
		mEditBox2DHeight->SetStep(1.f);

		Box2DInfo boxInfo = static_cast<CColliderBox2D*>(mComponent.Get())->GetInfo();
		mEditBox2DWidth->SetVal(boxInfo.Length.x);
		mEditBox2DHeight->SetVal(boxInfo.Length.y);

		mEditBox2DWidth->SetCallBack(this, &CColliderControlWidget::OnChangeBoxWidth);
		mEditBox2DHeight->SetCallBack(this, &CColliderControlWidget::OnChangeBoxHeight);
		break;
	}
	case eColliderType::Circle2D:
	{
		mTypeText = AddWidget<CIMGUIText>("text");
		mTypeText->SetText("CircleInfo");
		mEditCircleRadius = AddWidget<CIMGUIInputFloat>("Radius");
		mEditCircleRadius->SetStep(1.f);

		CircleInfo circleInfo = static_cast<CColliderCircle*>(mComponent.Get())->GetInfo();
		mEditCircleRadius->SetVal(circleInfo.Radius);

		mEditCircleRadius->SetCallBack(this, &CColliderControlWidget::OnChangeCircleRadius);
		break;
	}
	case eColliderType::Pixel:
	{
		mTypeText = AddWidget<CIMGUIText>("text");
		mTypeText->SetText("PixelInfo");
		break;
	}
	default:
		assert(false);
		break;
	}
	mSeperator = AddWidget<CIMGUISeperator>("seperator");

	// Widget 설정
	mColliderTypeWidget->AddItem("Box2D");
	mColliderTypeWidget->AddItem("Circle2D");
	mColliderTypeWidget->AddItem("Pixel");

	switch (meType)
	{
	case eColliderType::Box2D:
		mColliderTypeWidget->SetCurrentItem("Box2D");
		break;
	case eColliderType::Circle2D:
		mColliderTypeWidget->SetCurrentItem("Circle2D");
		break;
	case eColliderType::Pixel:
		mColliderTypeWidget->SetCurrentItem("Pixel");
		break;
	default:
		assert(false);
		break;
	}

	Vector3 offset = static_cast<CColliderComponent*>(mComponent.Get())->GetOffset();
	mOffsetWidget->SetVal(offset.x, offset.y, offset.z);

	std::vector<std::string> profiles;
	CCollisionManager::GetInst()->GetProfileNames(profiles);
	size_t size = profiles.size();
	for (size_t i = 0; i < size; ++i)
	{
		mCollsionProfileWidget->AddItem(profiles[i]);
	}

	CollisionProfile* profile = static_cast<CColliderComponent*>(mComponent.Get())->GetCollisionProfile();
	if (profile)
	{
		mCollsionProfileWidget->SetCurrentItem(profile->Name);
	}
	else
	{
		static_cast<CColliderComponent*>(mComponent.Get())->SetCollisionProfile("Object");
		mCollsionProfileWidget->SetCurrentItem("Object");
	}

	// Callback
	mColliderTypeWidget->SetSelectCallBack(this, &CColliderControlWidget::OnSelectColliderType);
	mOffsetWidget->SetCallBack(this, &CColliderControlWidget::OnChangeOffsetWidget);
	mCollsionProfileWidget->SetSelectCallBack(this, &CColliderControlWidget::OnSelectCollisionProfile);
	mEditCollisionProfileWidget->SetClickCallBack(this, &CColliderControlWidget::OnClickEditProfile);
	return true;
}

void CColliderControlWidget::OnSelectColliderType(int idx, const char* label)
{
	eColliderType select;

	if (0 == strcmp("Box2D", label))
	{
		select = eColliderType::Box2D;
	}
	else if (0 == strcmp("Circle2D", label))
	{
		select = eColliderType::Circle2D;
	}
	else if (0 == strcmp("Pixel", label))
	{
		select = eColliderType::Pixel;
	}
	else
	{
		assert(false);
	}

	if (select == meType)
	{
		return;
	}

	changeType(select);
}

void CColliderControlWidget::OnChangeOffsetWidget(float val[3])
{
	static_cast<CColliderComponent*>(mComponent.Get())->SetOffset(val[0], val[1], val[2]);
}

void CColliderControlWidget::OnSelectCollisionProfile(int idx, const char* label)
{
	static_cast<CColliderComponent*>(mComponent.Get())->SetCollisionProfile(label);
}

void CColliderControlWidget::OnClickEditProfile()
{
	CIMGUIManager::GetInst()->AddWindow<CCollisionProfileWindow>("Collision Profile");
}

void CColliderControlWidget::OnChangeBoxWidth(float val)
{
	static_cast<CColliderBox2D*>(mComponent.Get())->SetWidth(val);
}

void CColliderControlWidget::OnChangeBoxHeight(float val)
{
	static_cast<CColliderBox2D*>(mComponent.Get())->SetHeight(val);
}

void CColliderControlWidget::OnChangeCircleRadius(float val)
{
	static_cast<CColliderCircle*>(mComponent.Get())->SetRadius(val);
}

void CColliderControlWidget::changeType(eColliderType type)
{
	CGameObject* obj = mComponent->GetGameObject();

	// Collider List에서 제거
	mComponent->GetScene()->GetCollision()->DeleteCollider((CColliderComponent*)mComponent.Get());

	switch (type)
	{
	case eColliderType::Box2D:
		mComponent = obj->ReplaceComponent<CColliderBox2D>(mComponent, mComponent->GetName());
		break;
	case eColliderType::Circle2D:
		mComponent = obj->ReplaceComponent<CColliderCircle>(mComponent, mComponent->GetName());
		break;
	case eColliderType::Pixel:
		mComponent = obj->ReplaceComponent<CColliderPixel>(mComponent, mComponent->GetName());
		break;
	default:
		assert(false);
		break;
	}
	mComponent->Start();
	static_cast<CColliderComponent*>(mComponent.Get())->SetOffset(mOffsetWidget->GetX(), mOffsetWidget->GetY(), 0.f);

	// 이전 타입 위젯 삭제
	switch (meType)
	{
	case eColliderType::Box2D:
		DeleteWidget((CIMGUIWidget*)mEditBox2DWidth);
		DeleteWidget((CIMGUIWidget*)mEditBox2DHeight);
		DeleteWidget((CIMGUIWidget*)mSeperator);
		break;
	case eColliderType::Circle2D:
		DeleteWidget((CIMGUIWidget*)mEditCircleRadius);
		DeleteWidget((CIMGUIWidget*)mSeperator);
		break;
	case eColliderType::Pixel:
		break;
	default:
		break;
	}
	
	// 현재 타입 위젯 등록
	meType = type;

	if (eColliderType::Box2D == meType)
	{
		mTypeText->SetText("BoxInfo");
		mEditBox2DWidth = AddWidget<CIMGUIInputFloat>("Width");
		mEditBox2DHeight = AddWidget<CIMGUIInputFloat>("Height");

		Box2DInfo boxInfo = static_cast<CColliderBox2D*>(mComponent.Get())->GetInfo();
		mEditBox2DWidth->SetVal(boxInfo.Length.x);
		mEditBox2DHeight->SetVal(boxInfo.Length.y);
		mEditBox2DWidth->SetStep(1.f);
		mEditBox2DHeight->SetStep(1.f);

		mEditBox2DWidth->SetCallBack(this, &CColliderControlWidget::OnChangeBoxWidth);
		mEditBox2DHeight->SetCallBack(this, &CColliderControlWidget::OnChangeBoxHeight);
	}
	else if (eColliderType::Circle2D == meType)
	{
		mTypeText->SetText("CircleInfo");
		mEditCircleRadius = AddWidget<CIMGUIInputFloat>("Radius");
		mEditCircleRadius->SetStep(1.f);

		CircleInfo circleInfo = static_cast<CColliderCircle*>(mComponent.Get())->GetInfo();

		mEditCircleRadius->SetVal(circleInfo.Radius);
		mEditCircleRadius->SetCallBack(this, &CColliderControlWidget::OnChangeCircleRadius);
	}
	else if (eColliderType::Pixel == meType)
	{

	}
	mSeperator = AddWidget<CIMGUISeperator>("seperator");
}
