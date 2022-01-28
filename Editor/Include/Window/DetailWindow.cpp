#include "DetailWindow.h"
#include "IMGUITree.h"
#include "IMGUIInputFloat.h"
#include "IMGUIText.h"
#include "IMGUIChild.h"
#include "IMGUIListBox.h"
#include "IMGUIManager.h"
#include "ObjectHierachyWindow.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "../WIdget/ComponentControlWidget.h"
#include "../Widget/SpriteControlWidget.h"

CDetailWindow::CDetailWindow() :
    mPosXInput(nullptr),
    mPosYInput(nullptr),
    mPosZInput(nullptr)
{
}

CDetailWindow::~CDetailWindow()
{
}

bool CDetailWindow::Init()
{
    CIMGUIWindow::Init();

    CIMGUIChild* child = AddWidget<CIMGUIChild>("detailWindowChild1", 300.f, 300.f);
    child->EnableBorder(false);
    CIMGUITree* tree = child->AddWidget<CIMGUITree>("Transform");
    CIMGUIText* text = tree->AddWidget<CIMGUIText>("text");
    text->SetText("Position");
    mPosXInput = tree->AddWidget<CIMGUIInputFloat>("X", 150.f, 0.f);
    mPosXInput->SetVal(0.f);
    mPosXInput->SetCallBack(this, &CDetailWindow::OnPosXInputChanged);
    mPosYInput = tree->AddWidget<CIMGUIInputFloat>("Y", 150.f, 0.f);
    mPosYInput->SetVal(0.f);
    mPosYInput->SetCallBack(this, &CDetailWindow::OnPosYInputChanged);
    mPosZInput = tree->AddWidget<CIMGUIInputFloat>("Z", 150.f, 0.f);
    mPosZInput->SetVal(0.f);
    mPosZInput->SetCallBack(this, &CDetailWindow::OnPosZInputChanged);

    return true;
}

void CDetailWindow::Update(float deltaTime)
{
    CIMGUIWindow::Update(deltaTime);
}

void CDetailWindow::UpdateComponentUI(class CGameObject* obj)
{
    // 기존 Component List 비우기
    ClearWidget();

    // 오브젝트가 가진 모든 컴포넌트 포인터
    std::vector<CComponent*> vecComponents;
    obj->GetAllComponentsPointer(vecComponents);

    size_t size = vecComponents.size();

    // TODO : Sprite Control Window 만들고 테스트
    for (size_t i = 0; i < size; ++i)
    {
        // Component List에 종류 맞춰 추가
        CComponentControlWidget* widget = CreateControlWidget(vecComponents[i]);
        widget->mComponent = vecComponents[i];
        mVecWidget.push_back((CIMGUIWidget*)widget);
    }
}

void CDetailWindow::OnPosXInputChanged(float val)
{
    CObjectHierachyWindow* hierachy = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Object Hierachy");

    if (!hierachy)
    {
        return;
    }

    CGameObject* obj = nullptr;

    CIMGUIListBox* objList = hierachy->GetObjectListBox();

    if (objList->GetSelectIndex() == -1)
    {
        return;
    }

    obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

    if (!obj)
    {
        return;
    }
    
    Vector3 objPos = obj->GetRootSceneComponent()->GetWorldPos();
    objPos.x = val;

    obj->SetWorldPos(objPos);
}

void CDetailWindow::OnPosYInputChanged(float val)
{
    CObjectHierachyWindow* hierachy = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Object Hierachy");

    if (!hierachy)
    {
        return;
    }

    CGameObject* obj = nullptr;

    CIMGUIListBox* objList = hierachy->GetObjectListBox();

    if (objList->GetSelectIndex() == -1)
    {
        return;
    }

    obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

    if (!obj)
    {
        return;
    }
    
    Vector3 objPos = obj->GetRootSceneComponent()->GetWorldPos();
    objPos.y = val;

    obj->SetWorldPos(objPos);
}

void CDetailWindow::OnPosZInputChanged(float val)
{
    CObjectHierachyWindow* hierachy = (CObjectHierachyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("Object Hierachy");

    if (!hierachy)
    {
        return;
    }

    CGameObject* obj = nullptr;

    CIMGUIListBox* objList = hierachy->GetObjectListBox();

    if (objList->GetSelectIndex() == -1)
    {
        return;
    }

    obj = CSceneManager::GetInst()->GetScene()->FindObject(objList->GetSelectItem());

    if (!obj)
    {
        return;
    }
    
    Vector3 objPos = obj->GetRootSceneComponent()->GetWorldPos();
    objPos.z = val;

    obj->SetWorldPos(objPos);
}

CComponentControlWidget* CDetailWindow::CreateControlWidget(CComponent* comp)
{
    if (comp->GetTypeID() == typeid(CSpriteComponent).hash_code())
    {
        return (CComponentControlWidget*)(new CSpriteControlWidget);
    }
    else
    {
        assert(false);
        return nullptr;
    }
}
